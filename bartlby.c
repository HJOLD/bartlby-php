/*
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2003 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+

  $Id$ 
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_bartlby.h"


/* If you declare any globals in php_bartlby.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(bartlby)
*/

/* True global resources - no need for thread safety here */
static int le_bartlby;



/* {{{ bartlby_functions[]
 *
 * Every user visible function must have an entry in bartlby_functions[].
 */
function_entry bartlby_functions[] = {
	PHP_FE(confirm_bartlby_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_service,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_worker,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_info,	NULL)		/* For testing, remove later. */
	
	{NULL, NULL, NULL}	/* Must be the last line in bartlby_functions[] */
};
/* }}} */

/* {{{ bartlby_module_entry
 */
zend_module_entry bartlby_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"bartlby",
	bartlby_functions,
	PHP_MINIT(bartlby),
	PHP_MSHUTDOWN(bartlby),
	PHP_RINIT(bartlby),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(bartlby),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(bartlby),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BARTLBY
ZEND_GET_MODULE(bartlby)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("bartlby.global_value",      "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_bartlby_globals, bartlby_globals)
    STD_PHP_INI_ENTRY("bartlby.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_bartlby_globals, bartlby_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_bartlby_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_bartlby_init_globals(zend_bartlby_globals *bartlby_globals)
{
	bartlby_globals->global_value = 0;
	bartlby_globals->global_string = NULL;
}
*/
/* }}} */


char * getConfigValue(char * key, char * fname) {
	FILE * fp;
	char  str[1024];

	char * tok;
	
	fp=fopen(fname, "r");
	if(!fp) 
		return NULL;
	
	while(fgets(str,1024,fp) != NULL) {
		str[strlen(str)-1]='\0';
		tok=strtok(str, "=");
		if(tok != NULL) {
				if(strcmp(tok, key) == 0) {
						tok=strtok(NULL, "=");
						if(tok == NULL) {
								return NULL;
						}
						tok[strlen(tok)-1]='\0';
						return strdup(tok);
						
				} else {
					continue;
				}
					
		}
			
	}
	
	
	fclose(fp);
	
	
	return NULL;
}


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(bartlby)
{
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(bartlby, php_bartlby_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	*/
	
	return SUCCESS;
	
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(bartlby)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(bartlby)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(bartlby)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bartlby)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "bartlby support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_bartlby_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_bartlby_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char string[256];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = sprintf(string, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "bartlby", arg);
	RETURN_STRINGL(string, len, 1);
}


PHP_FUNCTION(bartlby_get_info) {
	
	char * shmtok;
	int shm_id;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	
	
	
	pval * bartlby_config;
	pval * bartlby_service_id;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config, &bartlby_service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_service_id);
	convert_to_string(bartlby_config);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shmtok = getConfigValue("shm_key", Z_STRVAL_P(bartlby_config));
	if(shmtok == NULL) {
		php_error(E_WARNING, "unset bartlbyconfig var shm_key");	
		RETURN_FALSE;
	}
	
	
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		shm_hdr=(struct shm_header *)(void *)bartlby_address;
		
		add_assoc_long(return_value, "services", shm_hdr->svccount);
		add_assoc_long(return_value, "workers", shm_hdr->wrkcount);
		add_assoc_long(return_value, "current_running", shm_hdr->current_running);
		shmdt(bartlby_address);
	
	} else {
		php_error(E_WARNING, "SHM segment is not existing (bartlby running?)");	
		free(shmtok);
		RETURN_FALSE;
	}
	free(shmtok);
	
}
PHP_FUNCTION(bartlby_get_service) {
	
	char * shmtok;
	int shm_id;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	
	struct service * svcmap;
	
	pval * bartlby_config;
	pval * bartlby_service_id;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config, &bartlby_service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_service_id);
	convert_to_string(bartlby_config);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shmtok = getConfigValue("shm_key", Z_STRVAL_P(bartlby_config));
	if(shmtok == NULL) {
		php_error(E_WARNING, "unset bartlbyconfig var shm_key");	
		RETURN_FALSE;
	}
	
	
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		shm_hdr=(struct shm_header *)(void *)bartlby_address;
		svcmap=(struct service *)(void *)bartlby_address+sizeof(struct shm_header);
		if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
			php_error(E_WARNING, "Service id out of bounds");	
			RETURN_FALSE;	
		}
		add_assoc_long(return_value, "server_id", svcmap[Z_LVAL_P(bartlby_service_id)].service_id);
		add_assoc_long(return_value, "last_state", svcmap[Z_LVAL_P(bartlby_service_id)].last_state);
		add_assoc_long(return_value, "last_state", svcmap[Z_LVAL_P(bartlby_service_id)].current_state);
		add_assoc_long(return_value, "client_port", svcmap[Z_LVAL_P(bartlby_service_id)].client_port);
				
		add_assoc_string(return_value, "new_server_text", svcmap[Z_LVAL_P(bartlby_service_id)].new_server_text, 1);
		add_assoc_string(return_value, "service_name", svcmap[Z_LVAL_P(bartlby_service_id)].service_name, 1);
		add_assoc_string(return_value, "server_name", svcmap[Z_LVAL_P(bartlby_service_id)].server_name, 1);
		add_assoc_string(return_value, "client_ip", svcmap[Z_LVAL_P(bartlby_service_id)].client_ip, 1);
		add_assoc_string(return_value, "plugin", svcmap[Z_LVAL_P(bartlby_service_id)].plugin, 1);
		add_assoc_string(return_value, "plugin_arguments", svcmap[Z_LVAL_P(bartlby_service_id)].plugin_arguments, 1);
		
		add_assoc_long(return_value, "check_interval", svcmap[Z_LVAL_P(bartlby_service_id)].check_interval);
		add_assoc_long(return_value, "last_check", svcmap[Z_LVAL_P(bartlby_service_id)].last_check);
		
		add_assoc_long(return_value, "hour_from", svcmap[Z_LVAL_P(bartlby_service_id)].hour_from);
		add_assoc_long(return_value, "min_from", svcmap[Z_LVAL_P(bartlby_service_id)].min_from);
		add_assoc_long(return_value, "hour_to", svcmap[Z_LVAL_P(bartlby_service_id)].hour_to);
		add_assoc_long(return_value, "min_to", svcmap[Z_LVAL_P(bartlby_service_id)].min_to);
		
		add_assoc_long(return_value, "notify_enabled", svcmap[Z_LVAL_P(bartlby_service_id)].notify_enabled);
		add_assoc_long(return_value, "last_notify_send", svcmap[Z_LVAL_P(bartlby_service_id)].last_notify_send);
		add_assoc_long(return_value, "flap_count", svcmap[Z_LVAL_P(bartlby_service_id)].flap_count);
		
		add_assoc_long(return_value, "service_type", svcmap[Z_LVAL_P(bartlby_service_id)].service_type);
		add_assoc_long(return_value, "service_passive_timeout", svcmap[Z_LVAL_P(bartlby_service_id)].service_passive_timeout);
		
		
		add_assoc_string(return_value, "service_var", svcmap[Z_LVAL_P(bartlby_service_id)].service_var, 1);
		shmdt(bartlby_address);
	/*	
		int service_id;
	int server_id;
	int last_state;
	int current_state;
	int client_port;
	char  new_server_text[2048];
	char  service_name[2048];
	char  server_name[2048];
	char  client_ip[2048];
	char  plugin[2048];
	char  plugin_arguments[2048];
	int check_interval;
	int last_check;
	
	
	
	int hour_from;
	int min_from;
	int hour_to;
	int min_to;
	
	
	int notify_enabled;
	int last_notify_send;
	int flap_count;
	
	char  service_var[2048];
	int service_type;
	int service_passive_timeout;
	*/
	
	} else {
		php_error(E_WARNING, "SHM segment is not existing (bartlby running?)");	
		free(shmtok);
		RETURN_FALSE;
	}
	free(shmtok);
	
}

PHP_FUNCTION(bartlby_get_worker) {
	char * shmtok;
	int shm_id;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	struct worker * wrkmap;
	struct service * svcmap;
	
	pval * bartlby_config;
	pval * bartlby_worker_id;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config, &bartlby_worker_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_worker_id);
	convert_to_string(bartlby_config);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shmtok = getConfigValue("shm_key", Z_STRVAL_P(bartlby_config));
	if(shmtok == NULL) {
		php_error(E_WARNING, "unset bartlbyconfig var shm_key");	
		RETURN_FALSE;
	}
	
	
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		shm_hdr=(struct shm_header *)(void *)bartlby_address;
		svcmap=(struct service *)(void *)bartlby_address+sizeof(struct shm_header);
		wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount]+20;
		
		if(Z_LVAL_P(bartlby_worker_id) > shm_hdr->wrkcount-1) {
			php_error(E_WARNING, "Worker id out of bounds");	
			RETURN_FALSE;	
		}
		
		
		add_assoc_string(return_value, "mail", wrkmap[Z_LVAL_P(bartlby_worker_id)].mail, 1);
		add_assoc_string(return_value, "icq", wrkmap[Z_LVAL_P(bartlby_worker_id)].icq, 1);
		add_assoc_string(return_value, "services", wrkmap[Z_LVAL_P(bartlby_worker_id)].services, 1);
		
		add_assoc_long(return_value, "icq_notify", wrkmap[Z_LVAL_P(bartlby_worker_id)].icq_notify);
		add_assoc_long(return_value, "mail_notify", wrkmap[Z_LVAL_P(bartlby_worker_id)].mail_notify);
		add_assoc_long(return_value, "escalation_count", wrkmap[Z_LVAL_P(bartlby_worker_id)].escalation_count);
		add_assoc_long(return_value, "escalation_time", wrkmap[Z_LVAL_P(bartlby_worker_id)].escalation_time);
		shmdt(bartlby_address);
	/*	
	char  mail[2048];
	char  icq[2048];
	char  services[2048];
	
	
	int icq_notify;
	int mail_notify;
	int escalation_count;
	int escalation_time;
	char t[500];
	*/
	
	} else {
		php_error(E_WARNING, "SHM segment is not existing (bartlby running?)");	
		free(shmtok);
		RETURN_FALSE;
	}
	free(shmtok);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
