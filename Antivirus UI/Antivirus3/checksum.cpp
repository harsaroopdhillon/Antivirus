#include <header.h>
#include "checksum.h"
checksum::checksum(QObject *parent)
    : QObject(parent)
{

}
/*
HANDLE ghPipe = INVALID_HANDLE_VALUE;
BOOL InitalizePipe(HANDLE hPipe){
    hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(hPipe != INVALID_HANDLE_VALUE)
        return TRUE;
    DWORD dwLastError = GetLastError();
    if(dwLastError == ERROR_PIPE_BUSY){
        return WaitNamedPipeW(PIPE_NAME, PIPE_WAIT_TIME);
    }
    return FALSE;
}

BOOL WriteFile_Pipe(HANDLE hPipe, LPCVOID lpData, DWORD dwNumberOfBytesToWrite){
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    DWORD dwBytesWritten = 0;
    BOOL rv = FALSE;
    rv = SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
    if(rv){
        //OVERLAPPED ol;
        //ol.hEvent;
        rv = WriteFile(hPipe, lpData, dwNumberOfBytesToWrite, &dwBytesWritten, NULL);
    }
    return rv;
}
BOOL ReadFile_Pipe(HANDLE hPipe, LPVOID lpDataRead, DWORD dwBytesRead){
    BOOL rv = FALSE;
    while(!rv){
        rv = ReadFile(hPipe, lpDataRead, CHUNK_SIZE, &dwBytesRead, NULL);
        if(!rv && GetLastError() != ERROR_MORE_DATA)
            break;
    }
    return rv;
}
BOOL Shutdown_Pipe(HANDLE hPipe){
    // cleanup here.
    return CloseHandle(hPipe);
}
*/
void checksum :: fileCheckHash(const QString &filename, int type)
{
  //  LPCWSTR lpszFileName = reinterpret_cast<LPCWSTR>(filename.utf16());
// send data to the pipe.
   // DWORD dwLen = (wcslen(lpszFileName) + 1) * sizeof(WCHAR);
    //BOOL rv = WriteFile_Pipe(ghPipe, lpszFileName, dwLen);

    emit finally();
}
// check DB for data
bool checksum :: CheckDataBase(QByteArray arr)
{
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName("Store.db");
        //todo add pass etc.
        db.open();
        if(!db.isOpen())
            return false;
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS fileHash "
                   "(fileHashVal varchar(70))");
        query = QSqlQuery();

        QString queryString = "SELECT * FROM fileHash WHERE fileHashVal = :hash";
        if(!query.prepare(queryString) )
            return false;

        query.bindValue(":hash", QString(arr));

        if( !query.exec() )
            return false;

        if(!query.next())
            return false;

        db.close();
        db.removeDatabase(DRIVER);
        return true;
    }
    return false;
}
