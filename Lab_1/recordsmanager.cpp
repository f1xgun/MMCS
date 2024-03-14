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
                this->records.push_back(AccountRecord(site_name, login, password));
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
    QByteArray encryptedBytes  = QByteArray::fromHex(hexEncryptedBytes);
    QByteArray decryptedBytes;
    decrypt_file(key, encryptedBytes, decryptedBytes);

    records_file.close();
    return decryptedBytes.trimmed().replace('\n', "\r\n");
}

std::vector<AccountRecord> RecordsManager::get_records() {
    return this->records;
}

std::vector<AccountRecord> RecordsManager::filter_records(std::string site_name) {
    std::vector<AccountRecord> filtered_records;
    std::copy_if(this->records.begin(), this->records.end(), std::back_insert_iterator(filtered_records), [site_name](AccountRecord record) {
        return record.get_site_name().find(site_name) != std::string::npos;
    });
    return filtered_records;
}

void RecordsManager::add_record(AccountRecord& record) {
    this->records.push_back(record);
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
    // EVP_CIPHER_CTX_set_padding(ctx, 0);
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
        qDebug() << "*** tmp_buf" << tmp_buf;
        decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), decr_len);
        encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    }

    qDebug() << "*** before EVP_DecryptFinal_ex" << decrypted_buffer.data();
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
    qDebug() << "*** after EVP_DecryptFinal_ex" << decr_len << reinterpret_cast<char*>(decrypted_buf);

    return 0;
}

QString RecordsManager::get_record_cred(const QString& password, int index) {
    QByteArray decrypted_login;
    QByteArray decrypted_password;
    QByteArray key = QCryptographicHash::hash(
                         password.toUtf8(),
                         QCryptographicHash::Sha256).toHex();
    int login_result = decrypt_file(key, QByteArray::fromHex(QByteArray::fromStdString(this->records[index].get_login())), decrypted_login);
    int password_result = decrypt_file(key, QByteArray::fromHex(QByteArray::fromStdString(this->records[index].get_password())), decrypted_password);
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

// iv = s[CK%fdTlI$Kfc.zgt\7K}:kk02?KJ9E
// pass = SHA256(123) = a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3
