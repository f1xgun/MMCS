#ifndef RECORDSMANAGER_H
#define RECORDSMANAGER_H

#include "accountrecord.h"
#include <vector>
#include <QString>

class RecordsManager
{
    std::vector<AccountRecord*> records;
    std::vector<AccountRecord*> filtered_records;
    QString get_records_json_string(const QByteArray& key);
    int decrypt_file(const QByteArray& key_hex, const QByteArray & encryptedBytes, QByteArray & decryptedBytes);
    int encrypt_file(const QByteArray& key_hex, const QByteArray & plainBytes, QByteArray & encryptedBytes);
    QByteArray key;
public:
    RecordsManager();

    bool load_records(const QByteArray& key);
    std::vector<AccountRecord *> get_records();
    void filter_records(std::string site_name);
    void add_record(AccountRecord& record);
    QString get_record_cred(const QByteArray& password, int index);
    void save_records();
    std::string encrypt_string(const std::string& str);
    void delete_record(int index);
};

#endif // RECORDSMANAGER_H
