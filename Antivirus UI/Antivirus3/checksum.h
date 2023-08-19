#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QObject>
#include <windows.h>
typedef struct _PIPE_DATA_WRITE{
    LPWSTR lpszFileName;
} PIPE_DATA_WRITE, *PPIPE_DATA_WRITE;
typedef struct _PIPE_DATA_READ {
    BOOL bIsMalware;
    LPWSTR lpszType;
    LPWSTR lpszDescription;
    LPWSTR lpszActionTaken;
} PIPE_DATA_READ, *PPIPE_DATA_READ;
typedef struct _PIPE_DATA_ACTION{
    BOOL bUpdate; // check for  updates on the side....
    BOOL bQuarantine;
    BOOL bRestore;
    BOOL bRemove;
} PIPE_DATA_ACTION, *PPIPE_DATA_ACTION;
#define PIPE_NAME (L"\\\\.\\pipe\\AntiVirus")
#define SECOND 1000
#define PIPE_WAIT_TIME SECOND*10
#define CHUNK_SIZE 1024
class checksum
        : public QObject
{
    Q_OBJECT

public:
    bool CheckDataBase(QByteArray);
    checksum(QObject *parent = 0);

public slots:
    void fileCheckHash(const QString &filename , int type = 0);

signals:
    void done(const QString& filen, QByteArray hash , bool found , int num);
    void finally();
    void starting(const QString& dir , int num);
};



#endif // CHECKSUM_H
