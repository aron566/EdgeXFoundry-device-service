/**                                                                             
 *  @file device_driver_event_db.c                                                   
 *                                                                              
 *  @date 2020年12月06日 22:46:39 星期天
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 设备事件数据库操作.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "device_driver_event_db.h"
#include "device_driver_list.h"
#include "device_driver_parse_par.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
#define ENABLE_POWER_OFF_STORE 1 /**< 1:断电存储使能 0:断电存储失能*/
/*整理数据库*/
#define VACUUM_DB "VACUUM;"
/*建表*/
/*地址号,参数名,当前值,时间戳 [PRIMARY KEY]*/
#define DB_CREATE_CMD "CREATE TABLE IF NOT EXISTS table_name(key1_name key1_type, \
															 key2_name key2_type, \
															 key3_name key3_type, \
															 key4_name key4_type, \
															 key5_name key5_type, \
															 key6_name key6_type);"
#define CREATE_DB(table_name, key1_name, key1_type, \
							  key2_name, key2_type, \
							  key3_name, key3_type, \
							  key4_name, key4_type, \
							  key5_name, key5_type, \
							  key6_name, key6_type) DB_CREATE_CMD
const char *CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS %s(addr char(254), \
															 paramName char(254), \
															 valueCurrent char(254), \
															 timeStamp char(254));";

/*删表*/
#define EMPTY_DB(table_name) "DELETE FROM table_name;"
const char *EMPTY_TABLE_SQL = "DELETE FROM %s;";

/*自增归零删除*/
#define DELETE_TABLE(table_name) "DELETE FROM sqlite_sequence WHERE name = 'table_name';"
const char *DELETE_TABLE_SQL = "DELETE FROM sqlite_sequence WHERE name = '%s';";

/*填表*/
#define INSERT_DB(table_name, key1_value, \
				key2_value, key3_value, \
				key4_value, key5_value, \
				key6_value) \
				"INSERT OR REPLACE INTO MAIN.table_name VALUES('key1_value', \
						'key2_value', 'key3_value', 'key4_value', 'key5_value', 'key6_value');"
const char *INSERT_TABLE = "INSERT OR REPLACE INTO MAIN.%s VALUES('%u', \
						'%s', '%s', '%lu');";

/*更新表*/
#define UPDATE_DB(table_name, key_name, condition_name1, condition_value1, condition_name2, condition_value2) \
				"UPDATE MAIN.table_name SET key_name = 'key_value' where condition_name = 'condition_value' AND condition_name2 = 'condition_value2';"
const char *UPDATE_TABLE = "UPDATE MAIN.%s SET valueCurrent = '%s', timeStamp = '%lu' where addr = '%u' AND paramName = '%s';";

/*customize set*/
const char *CUSTOMIZE_SET = "UPDATE MAIN.%s SET %s = '%s' where %s = '%s' AND %s = '%s';";

/*查表*/
#define SELECT_DB(table_name, condition_name1, condition_value1, condition_name2, condition_value2, condition_name3, condition_value3) \
				"SELECT * FROM MAIN.table_name WHERE condition_name1 = 'condition_value1' AND condition_name2 = 'condition_value2' AND condition_name3 = 'condition_value3';"
const char *SELECT_TABLE = "SELECT * FROM MAIN.%s WHERE addr = '%u' AND paramName = '%s';";

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static sqlite3 *db = NULL;
/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   建立设备数据库
  * @param   [in]table_name 数据库表名
  * @return  0建立成功
  * @author  aron566
  * @version V1.0
  * @date    2020-12-06
  ******************************************************************
  */
static int create_table(const char *table_name)
{
	char* 	errMsg		= NULL;
	int 	rc			= 0;
	char 	sqlCmd[512]	= {0};

	if(table_name == NULL)
	{
		return -1;
	}

	/*open database*/
	rc = sqlite3_open(DEV_EVENT_DB_NAME, &db);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "can't open database:%s\n", sqlite3_errmsg(db));
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return -1;
	}

	/*create table of database*/
	snprintf(sqlCmd, sizeof(sqlCmd), CREATE_TABLE_SQL, table_name);
	rc = sqlite3_exec(db, sqlCmd, NULL, NULL, &errMsg);
	if(SQLITE_OK != rc)
	{
		fprintf(stderr, "can't create file database :%s\n", errMsg);
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return -1;
	}
    
	/*empty table of database*/
#if !ENABLE_POWER_OFF_STORE
	snprintf(sqlCmd, sizeof(sqlCmd), DELETE_TABLE_SQL, table_name);
	rc = sqlite3_exec(db, sqlCmd, NULL, NULL, &errMsg);
	if(SQLITE_OK != rc)
	{
		fprintf(stderr, "can't empty file database :%s\n", errMsg);
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return -1;
	}

	/*整理*/
	rc = sqlite3_exec(db, VACUUM_DB, NULL, NULL, &errMsg);
	if(SQLITE_OK != rc)
	{
		fprintf(stderr, "can't empty file database :%s\n", errMsg);
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return -1;
	}
#endif
    return 0;
}

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/
/**
  ******************************************************************
  * @brief   数据录入
  * @param   [in]data 写入内容
  * @retval  -1异常 0正常写入
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
  ******************************************************************
  */
int dev_driver_event_db_record_insert(INSERT_DATA_Typedef_t *data)
{
	int      rc              =  0;
	char*    errMsg          =  NULL;
	char     sqlCmd[512]     =  {0};

	if(data == NULL || db == NULL)
	{
		return -1;
	}
	snprintf(sqlCmd, sizeof(sqlCmd) ,INSERT_TABLE ,data->table_name, 
									data->addr, data->param_name, 
									data->value_current, data->time_stamp);

	rc = sqlite3_exec(db, sqlCmd, NULL, NULL, &errMsg);
	if (SQLITE_OK != rc)
	{
		fprintf(stderr, "can't add record to memory database, sqlcmd=%s, err:%s\n", sqlCmd, errMsg);
		sqlite3_free(errMsg);
		return -1;
	}
	return 0;
}

/**
  ******************************************************************
  * @brief   更新数据库
  * @param   [in]data 更新内容
  * @retval  -1异常 0正常写入
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
  ******************************************************************
  */
int dev_driver_event_db_record_update(UPDATE_DATA_Typedef_t *data)
{
	int      rc              =  0;
	char*    errMsg          =  NULL;
	char     sqlCmd[512]     =  {0};

	if(data == NULL || db == NULL)
	{
		return -1;
	}
	snprintf(sqlCmd, sizeof(sqlCmd), UPDATE_TABLE ,data->table_name, data->value_current, 
									data->time_stamp, data->addr, data->param_name);

	rc = sqlite3_exec(db, sqlCmd, NULL, NULL, &errMsg);
	if (SQLITE_OK != rc)
	{
		fprintf(stderr, "can't update record to memory database, sqlcmd=%s, err:%s\n", sqlCmd, errMsg);
		sqlite3_free(errMsg);
		return -1;
	}
	return 0;
}

/**
  ******************************************************************
  * @brief   自定义更新字段
  * @param   [in]data 更新内容
  * @retval  -1异常 0正常写入
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
  ******************************************************************
  */
int dev_driver_event_db_record_customize_set(CUSTOMIZE_SET_Typedef_t *data)
{
	int      rc              =  0;
	char*    errMsg          =  NULL;
	char     sqlCmd[512]     =  {0};

	if(data == NULL || db == NULL)
	{
		return -1;
	}
	snprintf(sqlCmd, sizeof(sqlCmd), CUSTOMIZE_SET ,data->table_name, data->param_name1, 
									data->value1, data->condition_name1, data->condition_value1, 
									data->condition_name2, data->condition_value2);

	rc = sqlite3_exec(db, sqlCmd, NULL, NULL, &errMsg);
	if (SQLITE_OK != rc)
	{
		fprintf(stderr, "can't update record to memory database, sqlcmd=%s, err:%s\n", sqlCmd, errMsg);
		sqlite3_free(errMsg);
		return -1;
	}
	return 0;
}

/**
  ******************************************************************
  * @brief   查询数据
  * @param   [in]data 查询内容
  * @param   [in]get_result 查询结果回调接口
  * @retval  -1异常 0正常
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
  ******************************************************************
  */
int dev_driver_event_db_record_query(QUERY_DATA_Typedef_t *data, QUERY_CALLBACK get_result, void *callback_par)
{
	int      rc              = 0;
	char     *errMsg         = NULL;
	char**   pRecord         = NULL;
	int      row             = 0;
	int      column          = 0;
	char     sqlCmd[512]     = {0};

	if(data == NULL || db == NULL || get_result == NULL)
	{
		return -1;
	}
	snprintf(sqlCmd, sizeof(sqlCmd), SELECT_TABLE, data->table_name, data->addr, data->param_name);

	rc = sqlite3_get_table(db, sqlCmd, &pRecord, &row, &column, &errMsg);
	if (SQLITE_OK != rc) {
		fprintf(stderr, "can't get table %s\n",  errMsg);
		sqlite3_free(errMsg);
		return -1;
	}

	int ret = get_result(pRecord, row, column, callback_par);
	sqlite3_free_table(pRecord);

	return ret;
}

/**
  ******************************************************************
  * @brief   查询自定义数据
  * @param   [in]sql 查询内容sql语句
  * @param   [in]get_result 查询结果回调接口
  * @retval  -1异常 0正常
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
  ******************************************************************
  */
int dev_driver_event_db_record_customize_query(const char *sql, QUERY_CALLBACK get_result, void *callback_par)
{
	int      rc              = 0;
	char     *errMsg         = NULL;
	char**   pRecord         = NULL;
	int      row             = 0;
	int      column          = 0;
	char     sqlCmd[512]     = {0};

	if(sql == NULL || db == NULL || get_result == NULL)
	{
		return -1;
	}
	snprintf(sqlCmd, sizeof(sqlCmd), "%s", sql);

	rc = sqlite3_get_table(db, sqlCmd, &pRecord, &row, &column, &errMsg);
	if (SQLITE_OK != rc) {
		fprintf(stderr, "can't get table %s\n",  errMsg);
		sqlite3_free(errMsg);
		return -1;
	}

	int ret = get_result(pRecord, row, column, callback_par);
	sqlite3_free_table(pRecord);

	return ret;
}

/**
  ******************************************************************
  * @brief   初始化数据库，检测是否存在，不存在则建立
  * @param   [in]lc 日志
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-06
  ******************************************************************
  */
void dev_driver_event_db_init(iot_logger_t *lc)
{
	const DEVICE_TYPE_MAP_Typedef_t *table = get_device_type_list();
	if(table == NULL)
	{
		iot_log_error(lc, "can't find table.");
		return;
	}
	/*configuration sqlite to SQLITE_THREADSAFE*/
	sqlite3_config(SQLITE_CONFIG_SERIALIZED);

	/*建立设备事件表*/
	for(int index = 0; table[index].type_name != NULL; index++)
	{
		create_table(table[index].type_name);
	}
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                