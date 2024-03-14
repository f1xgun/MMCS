#ifndef RECORDSMANAGER_H
#define RECORDSMANAGER_H

#include "accountrecord.h"
#include <vector>
#include <QString>

class RecordsManager
{
    std::vector<AccountRecord> records;
    QString get_records_json_string(const QByteArray& key);
    int decrypt_file(const QByteArray& key_hex, const QByteArray & encryptedBytes, QByteArray & decryptedBytes);
    QByteArray key;
public:
    RecordsManager();

    bool load_records(const QByteArray& key);
    std::vector<AccountRecord> get_records();
    std::vector<AccountRecord> filter_records(std::string site_name);
    void add_record(AccountRecord& record);
    QString get_record_cred(const QString& password, int index);
};

#endif // RECORDSMANAGER_H
