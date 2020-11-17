/*
  MIT License

  Copyright (c) 2017 CK Tan
  https://github.com/cktan/tomlc99

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <inttypes.h>
#include "toml.h"
#include "../../custom-common.h"

static void print_escape_string(const char* s, char *outbuf)
{
	for ( ; *s; s++) {
		int ch = *s;
		switch (ch) {
		case '\b': sprintf(outbuf+strlen(outbuf), "\\b"); break;
		case '\t': sprintf(outbuf+strlen(outbuf), "\\t"); break;
		case '\n': sprintf(outbuf+strlen(outbuf), "\\n"); break;
		case '\f': sprintf(outbuf+strlen(outbuf), "\\f"); break;
		case '\r': sprintf(outbuf+strlen(outbuf), "\\r"); break;
		case '"': sprintf(outbuf+strlen(outbuf), "\\\""); break;
		case '\\': sprintf(outbuf+strlen(outbuf), "\\\\"); break;
		default: sprintf(outbuf+strlen(outbuf), "%c", ch); break;
		}
	}
}

static int print_raw(const char* s ,char *outbuf)
{
	char* sval;
	int64_t ival;
	int bval;
	double dval;
	toml_timestamp_t ts;
	char dbuf[100];

	if (0 == toml_rtos(s, &sval)) {
		sprintf(outbuf+strlen(outbuf), "{\"type\":\"string\",\"value\":\"");
		print_escape_string(sval ,outbuf);
		sprintf(outbuf+strlen(outbuf), "\"}");
		free(sval);
	} else if (0 == toml_rtoi(s, &ival)) {
		sprintf(outbuf+strlen(outbuf), "{\"type\":\"integer\",\"value\":\"%" PRId64 "\"}", ival);
	} else if (0 == toml_rtob(s, &bval)) {
		sprintf(outbuf+strlen(outbuf), "{\"type\":\"bool\",\"value\":\"%s\"}", bval ? "true" : "false");
	} else if (0 == toml_rtod_ex(s, &dval, dbuf, sizeof(dbuf))) {
		sprintf(outbuf+strlen(outbuf), "{\"type\":\"float\",\"value\":\"%s\"}", dbuf);
	} else if (0 == toml_rtots(s, &ts)) {
		char millisec[10];
		if (ts.millisec)
			sprintf(millisec, ".%d", *ts.millisec);
		else
			millisec[0] = 0;
		if (ts.year && ts.hour) {
			sprintf(outbuf+strlen(outbuf), "{\"type\":\"datetime\",\"value\":\"%04d-%02d-%02dT%02d:%02d:%02d%s%s\"}",
				   *ts.year, *ts.month, *ts.day, *ts.hour, *ts.minute, *ts.second,
				   millisec,
				   (ts.z ? ts.z : ""));
		} else if (ts.year) {
			sprintf(outbuf+strlen(outbuf), "{\"type\":\"date\",\"value\":\"%04d-%02d-%02d\"}",
				   *ts.year, *ts.month, *ts.day);
		} else if (ts.hour) {
			sprintf(outbuf+strlen(outbuf), "{\"type\":\"time\",\"value\":\"%02d:%02d:%02d%s\"}",
				   *ts.hour, *ts.minute, *ts.second, millisec);
		}
	} else {
		fprintf(stderr, "unknown type\n");
		return -1;
	}
	return 0;
}


static void print_array(toml_array_t* arr ,char *outbuf);
static void print_table(toml_table_t* curtab, char *outbuf)
{
	int i;
	const char* key;
	const char* raw;
	toml_array_t* arr;
	toml_table_t* tab;


	sprintf(outbuf+strlen(outbuf), "{");
	for (i = 0; 0 != (key = toml_key_in(curtab, i)); i++) {

		sprintf(outbuf+strlen(outbuf), "%s\"", i > 0 ? "," : "");
		print_escape_string(key, outbuf);
		sprintf(outbuf+strlen(outbuf), "\":");
	
		if (0 != (raw = toml_raw_in(curtab, key))) {
			print_raw(raw, outbuf);
		} else if (0 != (arr = toml_array_in(curtab, key))) {
			print_array(arr, outbuf);
		} else if (0 != (tab = toml_table_in(curtab, key))) {
			print_table(tab, outbuf);
		} else {
			printf("rint_table error.\n");
			abort();
		}
	}
	sprintf(outbuf+strlen(outbuf), "}");
}

static void print_table_array(toml_array_t* curarr, char *outbuf)
{
	int i;
	toml_table_t* tab;
	
	sprintf(outbuf+strlen(outbuf), "[");
	for (i = 0; 0 != (tab = toml_table_at(curarr, i)); i++) {
		sprintf(outbuf+strlen(outbuf), "%s", i > 0 ? "," : "");
		print_table(tab, outbuf);
	}
	sprintf(outbuf+strlen(outbuf), "]");
}

static void print_array(toml_array_t* curarr, char *outbuf)
{
	toml_array_t* arr;
	const char* raw;
	int i;

	if (toml_array_kind(curarr) == 't') {
		print_table_array(curarr, outbuf);
		return;
	} 

	sprintf(outbuf+strlen(outbuf), "{\"type\":\"array\",\"value\":[");
	switch (toml_array_kind(curarr)) {

	case 'v': 
		for (i = 0; 0 != (raw = toml_raw_at(curarr, i)); i++) {
			sprintf(outbuf+strlen(outbuf), "%s", i > 0 ? "," : "");
			print_raw(raw, outbuf);
		}
		break;

	case 'a': 
		for (i = 0; 0 != (arr = toml_array_at(curarr, i)); i++) {
			sprintf(outbuf+strlen(outbuf), "%s", i > 0 ? "," : "");
			print_array(arr, outbuf);
		}
		break;

	default:
		break;
	}
	sprintf(outbuf+strlen(outbuf), "]}");
}

static int cat(void* toml_ptr, CAT_TOML_TYPE type, char *outbuf)
{
	char  errbuf[200];
	
	toml_table_t* tab = NULL;
	switch(type)
	{
		case FP_TYPE:
			tab = toml_parse_file((FILE *)toml_ptr, errbuf, sizeof(errbuf));
			break;
		case STR_TYPE:
			tab = toml_parse((char *)toml_ptr, errbuf, sizeof(errbuf));
			break;
		default:
			return -1;
	}
	
	if (!tab) {
		fprintf(stderr, "ERROR: %s\n", errbuf);
		return -1;
	}

	print_table(tab ,outbuf);
	printf("\n");

	toml_free(tab);
	return 0;
}

/**
******************************************************************
* @brief   toml文件转为json格式
* @param   [in]toml文件名.
* @retval  正常返回0.
* @author  aron566
* @version V1.0
* @date    2020-11-15
******************************************************************
*/
int toml_file2json(char *toml_fileanme, char *outbuf)
{
	if(toml_fileanme == NULL || outbuf == NULL)
	{
		return -1;
	}
	FILE* fp = fopen(toml_fileanme, "r");
	if (!fp) {
		fprintf(stderr, "ERROR: cannot open %s: %s\n",
				toml_fileanme, strerror(errno));
		return -1;
	}
	cat(fp, FP_TYPE, outbuf);
	fclose(fp);
	return 0;
}

/**
******************************************************************
* @brief   toml字符串转为json格式
* @param   [in]toml字符串.
* @param   [out]outbuf 输出json字符串存储区.
* @retval  正常返回0.
* @author  aron566
* @version V1.0
* @date    2020-11-15
******************************************************************
*/
int toml2json(char *toml_str, char *outbuf)
{
	if(toml_str == NULL || outbuf == NULL)
	{
		return -1;
	}
	cat(toml_str, STR_TYPE ,outbuf);
	return 0;
}