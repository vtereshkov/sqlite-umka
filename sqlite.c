#include <string.h>

#include "sqlite3.h"
#include "umka_api.h"


typedef struct
{
    sqlite3 *handle;
} Database;


typedef struct
{
    sqlite3_stmt *stmt;
} Row;


UMKA_EXPORT void umSqliteErrStr(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    const int code = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->ptrVal = api->umkaMakeStr(umka, sqlite3_errstr(code));
}


static void umSqliteClose(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    Database *db = api->umkaGetParam(params, 0)->ptrVal;

    sqlite3_close(db->handle);
}


UMKA_EXPORT void umSqliteOpen(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    const char *name = api->umkaGetParam(params, 0)->ptrVal;
    Database **db = api->umkaGetParam(params, 1)->ptrVal;

    *db = api->umkaAllocData(umka, sizeof(Database), umSqliteClose);

    api->umkaGetResult(params, result)->intVal = sqlite3_open(name, &(*db)->handle);
}


static void umSqliteFinalize(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    Row *row = api->umkaGetParam(params, 0)->ptrVal;

    sqlite3_finalize(row->stmt);
}


UMKA_EXPORT void umSqlitePrepare(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);     

    Database *db = api->umkaGetParam(params, 0)->ptrVal;
    const char *request = api->umkaGetParam(params, 1)->ptrVal;
    Row **row = api->umkaGetParam(params, 2)->ptrVal;

    *row = api->umkaAllocData(umka, sizeof(Row), umSqliteFinalize);    

    api->umkaGetResult(params, result)->intVal = sqlite3_prepare_v2(db->handle, request, -1, &(*row)->stmt, NULL);
}


UMKA_EXPORT void umSqliteStep(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    Row *row = api->umkaGetParam(params, 0)->ptrVal;

    api->umkaGetResult(params, result)->intVal = sqlite3_step(row->stmt);
}


UMKA_EXPORT void umSqliteColumnCount(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);    

    Row *row = api->umkaGetParam(params, 0)->ptrVal;

    api->umkaGetResult(params, result)->intVal = sqlite3_column_count(row->stmt);
}


UMKA_EXPORT void umSqliteColumnName(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);     

    Row *row = api->umkaGetParam(params, 0)->ptrVal;
    const int col = api->umkaGetParam(params, 1)->intVal;

    api->umkaGetResult(params, result)->ptrVal = api->umkaMakeStr(umka, sqlite3_column_name(row->stmt, col));
}


UMKA_EXPORT void umSqliteColumnType(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);     

    Row *row = api->umkaGetParam(params, 0)->ptrVal;
    const int col = api->umkaGetParam(params, 1)->intVal;

    api->umkaGetResult(params, result)->intVal = sqlite3_column_type(row->stmt, col);
}


UMKA_EXPORT void umSqliteColumnInt64(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);     

    Row *row = api->umkaGetParam(params, 0)->ptrVal;
    const int col = api->umkaGetParam(params, 1)->intVal;

    api->umkaGetResult(params, result)->intVal = sqlite3_column_int64(row->stmt, col);
}


UMKA_EXPORT void umSqliteColumnDouble(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    Row *row = api->umkaGetParam(params, 0)->ptrVal;
    const int col = api->umkaGetParam(params, 1)->intVal;

    api->umkaGetResult(params, result)->realVal = sqlite3_column_double(row->stmt, col);
}


UMKA_EXPORT void umSqliteColumnText(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);     

    Row *row = api->umkaGetParam(params, 0)->ptrVal;
    const int col = api->umkaGetParam(params, 1)->intVal;

    api->umkaGetResult(params, result)->ptrVal = api->umkaMakeStr(umka, sqlite3_column_text(row->stmt, col));
}


UMKA_EXPORT void umSqliteColumnBlob(UmkaStackSlot *params, UmkaStackSlot *result)
{
    Umka *umka = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);    

    Row *row = api->umkaGetParam(params, 0)->ptrVal;
    const int col = api->umkaGetParam(params, 1)->intVal;
    const UmkaType *type = api->umkaGetParamType(params, 1);

    typedef UmkaDynArray(unsigned char) ByteArray;
    ByteArray *array = api->umkaGetResult(params, result)->ptrVal;

    const void *blob = sqlite3_column_blob(row->stmt, col);
    const int size = sqlite3_column_bytes(row->stmt, col);

    api->umkaMakeDynArray(umka, array, type, size);

    if (blob)
        memcpy(array->data, blob, size);    
}
