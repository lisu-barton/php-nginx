/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2015 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Sterling Hughes <sterling@php.net>                           |
   |         Wez Furlong <wez@thebrainroom.com>                           |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef _PHP_PCURL_H
#define _PHP_PCURL_H

#include "php.h"
#include "zend_smart_str.h"

#ifdef COMPILE_DL_PCURL
#undef HAVE_CURL
#define HAVE_CURL 1
#endif

#if HAVE_CURL

#define PHP_CURL_DEBUG 0

#ifdef PHP_WIN32
# define PHP_CURL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_CURL_API __attribute__ ((visibility("default")))
#else
# define PHP_CURL_API
#endif

#include "php_version.h"
#define PHP_CURL_VERSION PHP_VERSION

#include <curl/curl.h>
#include <curl/multi.h>

extern zend_module_entry pcurl_module_entry;
#define pcurl_module_ptr &pcurl_module_entry

#define CURLOPT_RETURNTRANSFER 19913
#define CURLOPT_BINARYTRANSFER 19914 /* For Backward compatibility */
#define PHP_CURL_STDOUT 0
#define PHP_CURL_FILE   1
#define PHP_CURL_USER   2
#define PHP_CURL_DIRECT 3
#define PHP_CURL_RETURN 4
#define PHP_CURL_IGNORE 7

extern int  le_pcurl;
#define le_pcurl_name "pcURL handle"
extern int  le_pcurl_multi_handle;
#define le_pcurl_multi_handle_name "pcURL Multi Handle"
extern int  le_pcurl_share_handle;
#define le_pcurl_share_handle_name "pcURL Share Handle"

PHP_MINIT_FUNCTION(pcurl);
PHP_MSHUTDOWN_FUNCTION(pcurl);
PHP_MINFO_FUNCTION(pcurl);

PHP_FUNCTION(pcurl_which_one);
PHP_FUNCTION(pcurl_fetch_one);
PHP_FUNCTION(pcurl_close);
PHP_FUNCTION(pcurl_copy_handle);
PHP_FUNCTION(pcurl_errno);
PHP_FUNCTION(pcurl_error);
PHP_FUNCTION(pcurl_exec);
PHP_FUNCTION(pcurl_getinfo);
PHP_FUNCTION(pcurl_init);
PHP_FUNCTION(pcurl_setopt);
PHP_FUNCTION(pcurl_setopt_array);
PHP_FUNCTION(pcurl_version);

PHP_FUNCTION(pcurl_multi_add_handle);
PHP_FUNCTION(pcurl_multi_close);
PHP_FUNCTION(pcurl_multi_exec);
PHP_FUNCTION(pcurl_multi_getcontent);
PHP_FUNCTION(pcurl_multi_info_read);
PHP_FUNCTION(pcurl_multi_init);
PHP_FUNCTION(pcurl_multi_remove_handle);
PHP_FUNCTION(pcurl_multi_select);

PHP_FUNCTION(pcurl_share_close);
PHP_FUNCTION(pcurl_share_init);
PHP_FUNCTION(pcurl_share_setopt);

#if LIBCURL_VERSION_NUM >= 0x070c00 /* 7.12.0 */
PHP_FUNCTION(pcurl_strerror);
PHP_FUNCTION(pcurl_multi_strerror);
#endif

#if LIBCURL_VERSION_NUM >= 0x070c01 /* 7.12.1 */
PHP_FUNCTION(pcurl_reset);
#endif

#if LIBCURL_VERSION_NUM >= 0x070f04 /* 7.15.4 */
PHP_FUNCTION(pcurl_escape);
PHP_FUNCTION(pcurl_unescape);

PHP_FUNCTION(pcurl_multi_setopt);
#endif

#if LIBCURL_VERSION_NUM >= 0x071200 /* 7.18.0 */
PHP_FUNCTION(pcurl_pause);
#endif
PHP_FUNCTION(pcurl_file_create);


void _php_pcurl_multi_close(zend_resource *);
void _php_pcurl_share_close(zend_resource *);

typedef struct {
	zval                  func_name;
	zend_fcall_info_cache fci_cache;
	FILE                 *fp;
	smart_str             buf;
	int                   method;
	zval					stream;
} php_pcurl_write;

typedef struct {
	zval                  func_name;
	zend_fcall_info_cache fci_cache;
	FILE                 *fp;
	zend_resource        *res;
	int                   method;
	zval                  stream;
} php_pcurl_read;

typedef struct {
	zval                  func_name;
	zend_fcall_info_cache fci_cache;
	int                   method;
} php_pcurl_progress, php_pcurl_fnmatch;

typedef struct {
	php_pcurl_write    *write;
	php_pcurl_write    *write_header;
	php_pcurl_read     *read;
#if CURLOPT_PASSWDFUNCTION != 0
	zval               passwd;
#endif
	zval               std_err;
	php_pcurl_progress *progress;
#if LIBCURL_VERSION_NUM >= 0x071500 /* Available since 7.21.0 */
	php_pcurl_fnmatch  *fnmatch;
#endif
} php_pcurl_handlers;

struct _php_pcurl_error  {
	char str[CURL_ERROR_SIZE + 1];
	int  no;
};

struct _php_pcurl_send_headers {
	zend_string *str;
};

struct _php_pcurl_free {
	zend_llist str;
	zend_llist post;
	HashTable *slist;
};

typedef struct {
	CURL                         *cp;
	php_pcurl_handlers            *handlers;
	zend_resource                *res;
	struct _php_pcurl_free        *to_free;
	struct _php_pcurl_send_headers header;
	struct _php_pcurl_error        err;
	zend_bool                     in_callback;
	uint32_t                      clone;
} php_pcurl;
typedef struct _pcurl_node {
	CURL *cp;
	struct _pcurl_node *next;
} pcurl_node;

typedef struct {
	int size;
	pcurl_node *all_link;
	pcurl_node *idle_link;
} _pcurl_pool;

#define PCURL_POOL_MAX 128
#define PCURL_POOL_INC 16
#define PCURL_HASH_PREFIX "pcurl_"

#define CURLOPT_SAFE_UPLOAD -1

typedef struct {
	int         still_running;
	CURLM      *multi;
	zend_llist  easyh;
} php_pcurlm;

typedef struct {
	CURLSH                   *share;
} php_pcurlsh;

void _php_pcurl_cleanup_handle(php_pcurl *);
void _php_pcurl_multi_cleanup_list(void *data);
void _php_pcurl_verify_handlers(php_pcurl *ch, int reporterror);

void pcurlfile_register_class(void);
PHP_CURL_API extern zend_class_entry *curl_pCURLFile_class;

#else
#define pcurl_module_ptr NULL
#endif /* HAVE_CURL */
#define phpext_pcurl_ptr pcurl_module_ptr
#endif  /* _PHP_CURL_H */
