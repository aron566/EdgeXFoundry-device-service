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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include "toml.h"
#include "../../custom-common.h"

typedef struct node_t node_t;
struct node_t {
    const char*   key;
    toml_table_t* tab;
};

node_t stack[20];
int stacktop = 0;

static void print_array_of_tables(toml_array_t* arr, const char* key);
static void print_array(toml_array_t* arr);

/**
 * @brief 打印表名
 * 
 * @param arrname 数组名或者是表名
 */
static void print_table_title(const char* arrname)
{
    int i;
    printf("%s", arrname ? "[[" : "[");
	/*打印上级*/
    for (i = 1; i < stacktop; i++) {
	printf("%s", stack[i].key);
	if (i + 1 < stacktop)
	    printf(".");
    }
	/*打印尾部*/
    if (arrname)
	printf(".%s]]\n", arrname);
    else
	printf("]\n");
}

/**
 * @brief 打印表
 * 
 * @param curtab 
 */
static void print_table(toml_table_t* curtab)
{
    int i;
    const char* key;
    const char* raw;
    toml_array_t* arr;
    toml_table_t* tab;

	/*轮询KEY*/
    for (i = 0; 0 != (key = toml_key_in(curtab, i)); i++) {
		/*如果是KV则打印*/
	if (0 != (raw = toml_raw_in(curtab, key))) {
	    printf("%s = %s\n", key, raw);
		/*如果是表中数组或者KV型数组*/
	} else if (0 != (arr = toml_array_in(curtab, key))) {
		/*[[key]]数组*/
	    if (toml_array_kind(arr) == 't') {
		print_array_of_tables(arr, key);
	    }
	    else {
			/*KV型数组*/
		printf("%s = [\n", key);/*xx = [ss,sss]*/
		print_array(arr);
		printf("    ]\n");
	    }
	} else if (0 != (tab = toml_table_in(curtab, key))) {
		/*如果是[tab.tab]则分解*/
	    stack[stacktop].key = key;
	    stack[stacktop].tab = tab;
	    stacktop++;
	    print_table_title(0);
	    print_table(tab);
	    stacktop--;
	} else {
	    abort();
	}
    }
}

/**
 * @brief 打印标表中数组
 * 
 * @param arr 数组指针
 * @param key 数组名
 */
static void print_array_of_tables(toml_array_t* arr, const char* key)
{
    int i;
    toml_table_t* tab;
    printf("\n");
	/*轮询表中数组*/
    for (i = 0; 0 != (tab = toml_table_at(arr, i)); i++) {
	print_table_title(key);//[[key]]
	print_table(tab);
	printf("\n");
    }
}

/**
 * @brief 打印数组
 * 
 * @param curarr 
 */
static void print_array(toml_array_t* curarr)
{
    toml_array_t* arr;
    const char* raw;
    toml_table_t* tab;
    int i;

    switch (toml_array_kind(curarr)) {
/*
[
	0： yyy,
	1： xxx,
	2： xxx,
]
*/
    case 'v': 
	for (i = 0; 0 != (raw = toml_raw_at(curarr, i)); i++) {
	    printf("  %d: %s,\n", i, raw);
	}
	break;
/*
[
	0：
		0：
			0：xxx
			1：xxx
		1：
			0：xxx
			1：xxx
	1：
		0：
			0：xxx
			1：xxx
		1：
			0：xxx
			1：xxx	
]
*/
    case 'a': 
	for (i = 0; 0 != (arr = toml_array_at(curarr, i)); i++) {
	    printf("  %d: \n", i);
	    print_array(arr);
	}
	break;
	    
    case 't': 
	for (i = 0; 0 != (tab = toml_table_at(curarr, i)); i++) {
	    print_table(tab);
	}
	printf("\n");
	break;
	
    case '\0':
	break;

    default:
	abort();
    }
}

/**
 * @brief 解析打印toml文件
 * 
 * @param fp 
 */
static void cat(FILE* fp)
{
    char  errbuf[200];
    
    toml_table_t* tab = toml_parse_file(fp, errbuf, sizeof(errbuf));
    if (!tab) {
	fprintf(stderr, "ERROR: %s\n", errbuf);
	return;
    }

    stack[stacktop].tab = tab;
    stack[stacktop].key = "";
    stacktop++;
    print_table(tab);
    stacktop--;

    toml_free(tab);
}


/**
******************************************************************
* @brief   toml文件打印
* @param   [in]toml文件名.
* @retval  None.
* @author  aron566
* @version V1.0
* @date    2020-11-15
******************************************************************
*/
int toml_file_cat(const char *toml_filename)
{	    
	FILE* fp = fopen(toml_filename, "r");
	if (!fp) {
	fprintf(stderr, "ERROR: cannot open %s: %s\n",
		toml_filename, strerror(errno));
		return -1;
	}
	cat(fp);
	fclose(fp);
    return 0;
}
