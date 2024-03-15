#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include <QApplication>
#include <QBuffer>
#include <QCryptographicHash>
#include "recordsmanager.h"
#include <openssl/evp.h>

RecordsManager::RecordsManager() {}

bool RecordsManager::load_records(const QByteArray& key) {
    this->key = key;
    QString records_json_string = get_records_json_string(key);
    QJsonParseError error;
    QJsonDocument records_doc = QJsonDocument::fromJson(records_json_string.toUtf8(), &error);
    qDebug() << "Error: " << error.errorString() << error.offset << error.error;
    if (records_doc.isArray()) {
        QJsonArray recods_json = records_doc.array();
        foreach (const QJsonValue& value, recods_json) {
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                std::string site_name = obj["site_name"].toString().toStdString();
                std::string login = obj["login"].toString().toStdString();
                std::string password = obj["password"].toString().toStdString();
                AccountRecord* record = new AccountRecord(site_name, login, password);
                this->records.push_back(record);
                this->filtered_records.push_back(record);
            }
        }
        return true;
    }
    return false;
}

QString RecordsManager::get_records_json_string(const QByteArray& key) {
    QFile records_file("records.json");
    records_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray hexEncryptedBytes = records_file.readAll();
    QByteArray encryptedBytes = QByteArray::fromHex(hexEncryptedBytes);
    QByteArray decryptedBytes;
    decrypt_file(key, encryptedBytes, decryptedBytes);

    records_file.close();
    return decryptedBytes.trimmed().replace('\n', "\r\n");
}

std::vector<AccountRecord*> RecordsManager::get_records() {
    return this->filtered_records;
}

void RecordsManager::filter_records(std::string site_name) {
    std::vector<AccountRecord*> filtered_records;
    std::copy_if(this->records.begin(), this->records.end(), std::back_insert_iterator(filtered_records), [site_name](AccountRecord* record) {
        return record->get_site_name().find(site_name) != std::string::npos;
    });
    this->filtered_records = filtered_records;
}

void RecordsManager::add_record(AccountRecord& record) {
    this->records.push_back(&record);
}

int RecordsManager::decrypt_file(const QByteArray& key_hex, const QByteArray & encryptedBytes, QByteArray & decryptedBytes) {
    QByteArray key_ba = QByteArray::fromHex(key_hex);
    unsigned char key[32] = {0};
    memcpy(key, key_ba.data(), 32);

    QByteArray iv_hex("00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f");
    QByteArray iv_ba = QByteArray::fromHex(iv_hex);
    unsigned char iv[16] = {0};
    memcpy(iv, iv_ba.data(), 16);


    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        qDebug() << "*** EVP_DecryptInit_ex2() ERROR";
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

#define BUF_LEN 256
    unsigned char encrypted_buf[BUF_LEN] = {0}, decrypted_buf[BUF_LEN] = {0};
    int encr_len, decr_len;
    QDataStream encrypted_stream(encryptedBytes);
    decryptedBytes.clear();

    QBuffer decrypted_buffer(&decryptedBytes);
    decrypted_buffer.open(QBuffer::WriteOnly);
    encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    while(encr_len > 0) {
        if (!EVP_DecryptUpdate(ctx, decrypted_buf, &decr_len, encrypted_buf, encr_len)) {
            /* Error */
            decrypted_buffer.close();
            EVP_CIPHER_CTX_free(ctx);
            return -1;
        }
        QByteArray tmp_buf(reinterpret_cast<char*>(decrypted_buf), decr_len);
        decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), decr_len);
        encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    }

    int tmplen;
    if (!EVP_DecryptFinal_ex(ctx, decrypted_buf, &tmplen)) {
        qDebug() << "*** EVP_DecryptFinal_ex() ERROR";
        decrypted_buffer.close();
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), tmplen);
    decrypted_buffer.close();
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

int RecordsManager::encrypt_file(const QByteArray& key_hex, const QByteArray & plainBytes, QByteArray & encryptedBytes) {
    QByteArray key_ba = QByteArray::fromHex(key_hex);
    unsigned char key[32] = {0};
    memcpy(key, key_ba.data(), key_ba.size());

    QByteArray iv_hex("00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f");
    QByteArray iv_ba = QByteArray::fromHex(iv_hex);
    unsigned char iv[16] = {0};
    memcpy(iv, iv_ba.data(), iv_ba.size());

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        qDebug() << "*** EVP_EncryptInit_ex() ERROR";
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

#define BUF_LEN 256
    unsigned char plain_buf[BUF_LEN] = {0}, encrypted_buf[BUF_LEN + EVP_MAX_BLOCK_LENGTH] = {0};
    int plain_len, encr_len;
    QDataStream plain_stream(plainBytes);
    encryptedBytes.clear();

    QBuffer encrypted_buffer(&encryptedBytes);
    encrypted_buffer.open(QBuffer::WriteOnly);

    while((plain_len = plain_stream.readRawData(reinterpret_cast<char*>(plain_buf), BUF_LEN)) > 0) {
        if (!EVP_EncryptUpdate(ctx, encrypted_buf, &encr_len, plain_buf, plain_len)) {
            /* Error */
            encrypted_buffer.close();
            EVP_CIPHER_CTX_free(ctx);
            return -1;
        }
        encrypted_buffer.write(reinterpret_cast<char*>(encrypted_buf), encr_len);
    }

    if (!EVP_EncryptFinal_ex(ctx, encrypted_buf, &encr_len)) {
        qDebug() << "*** EVP_EncryptFinal_ex() ERROR";
        encrypted_buffer.close();
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    encrypted_buffer.write(reinterpret_cast<char*>(encrypted_buf), encr_len);
    encrypted_buffer.close();
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

QString RecordsManager::get_record_cred(const QByteArray& key, int index) {
    QByteArray decrypted_login;
    QByteArray decrypted_password;
    int login_result = decrypt_file(key, QByteArray::fromHex(QByteArray::fromStdString(this->filtered_records[index]->get_login())), decrypted_login);
    int password_result = decrypt_file(key, QByteArray::fromHex(QByteArray::fromStdString(this->filtered_records[index]->get_password())), decrypted_password);
    if (login_result == -1 || password_result == -1) {
        return "ERROR";
    }

    QJsonObject credObject;
    credObject.insert("login", QString(decrypted_login));
    credObject.insert("password", QString(decrypted_password));

    QJsonDocument doc(credObject);
    QString jsonString = QString(doc.toJson(QJsonDocument::Compact));

    return jsonString;
}

void RecordsManager::save_records() {
    QJsonArray recordsArray;
    for (AccountRecord* record : this->records) {
        QJsonObject recordObject;
        recordObject["site_name"] = QString::fromStdString(record->get_site_name());
        recordObject["login"] = QString::fromStdString(record->get_login());
        recordObject["password"] = QString::fromStdString(record->get_password());
        recordsArray.append(recordObject);
    }

    QJsonDocument recordsDoc(recordsArray);
    QString recordsJsonString = recordsDoc.toJson(QJsonDocument::Compact);

    QByteArray data = recordsJsonString.toUtf8();

    QByteArray encryptedBytes;
    this->encrypt_file(key, data, encryptedBytes);

    QFile records_file("records.json");
    if (records_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (!records_file.write(encryptedBytes.toHex())) {
            qDebug() << "Error writing to file:" << records_file.errorString();
        }
        records_file.flush();
        records_file.close();
    }
}

std::string RecordsManager::encrypt_string(const std::string& str) {
    QByteArray encryptedString;
    encrypt_file(key, QString::fromStdString(str).toUtf8(), encryptedString);
    return encryptedString.toHex().toStdString();
}

void RecordsManager::delete_record(int index) {
    AccountRecord* deleted_record = this->filtered_records.at(index);
    for (int i = 0; i < this->records.size(); ++i) {
        if (deleted_record == this->records[i]) {
            this->records.erase(this->records.begin() + i);
            this->filtered_records.erase(this->filtered_records.begin() + index);
            break;
        }
    }
}
