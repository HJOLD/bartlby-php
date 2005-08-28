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
	PHP_FE(bartlby_version,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_config,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_lib_info, NULL)
	
	PHP_FE(bartlby_add_server,NULL)
	PHP_FE(bartlby_delete_server,NULL)
	PHP_FE(bartlby_modify_server,NULL)
	PHP_FE(bartlby_get_server_by_id,NULL)
	
	
	PHP_FE(bartlby_add_service,NULL)
	PHP_FE(bartlby_delete_service,NULL)
	PHP_FE(bartlby_modify_service,NULL)
	PHP_FE(bartlby_get_service_by_id,NULL)
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
	if(!fp) {
		printf("fopen %s failed\n", fname);
		return NULL;
	}
	
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

void * bartlby_get_sohandle(char * cfgfile) {
	char * data_lib;
	char * dlmsg;
	void * SOHandle;
	
	data_lib=getConfigValue("data_library", cfgfile);
	if(data_lib == NULL) {
		php_error(E_WARNING, "cannot find data_lib key in %s", cfgfile);	
		return NULL;
	}
	SOHandle=dlopen(data_lib, RTLD_LAZY);
	if((dlmsg=dlerror()) != NULL) {
		php_error(E_ERROR, "DL Error: %s", dlmsg);
        	return NULL;
    	}	
    	free(data_lib);
    	return SOHandle;
}

void * bartlby_get_shm(char * cfgfile) {
	char * shmtok;
	void * bartlby_address;
	int shm_id;
	
	shmtok = getConfigValue("shm_key", cfgfile);
	if(shmtok == NULL) {
		return NULL;	
	}		
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	free(shmtok);
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		return bartlby_address;
	} else {
		printf("EEEER");
		return NULL;
	}
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

PHP_FUNCTION(bartlby_version) {
	RETURN_STRING(BARTLBY_VERSION,1);	
	
}

PHP_FUNCTION(bartlby_config) {
	pval * bartlby_config;
	pval * bartlby_key;
	char * k;
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config, &bartlby_key)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	convert_to_string(bartlby_config);	
	convert_to_string(bartlby_key);	
	
	k=getConfigValue(Z_STRVAL_P(bartlby_key), Z_STRVAL_P(bartlby_config));
	if(k == NULL) {
		
		RETURN_FALSE;
	} else {
		RETURN_STRING(k,1);	
	}
	
	
}

PHP_FUNCTION(bartlby_get_info) {
	
	char * shmtok;
	int shm_id;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	
	
	
	pval * bartlby_config;
	pval * bartlby_service_id;
	
	if (ZEND_NUM_ARGS() != 1 || getParameters(ht, 1, &bartlby_config)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	convert_to_string(bartlby_config);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	bartlby_address=bartlby_get_shm(Z_STRVAL_P(bartlby_config));
	if(bartlby_address != NULL) {
		shm_hdr=(struct shm_header *)(void *)bartlby_address;
		
		add_assoc_long(return_value, "services", shm_hdr->svccount);
		add_assoc_long(return_value, "workers", shm_hdr->wrkcount);
		add_assoc_long(return_value, "current_running", shm_hdr->current_running);
		//add_assoc_long(return_value, "version", shm_hdr->version);
		add_assoc_string(return_value, "version", shm_hdr->version, 1);
		shmdt(bartlby_address);
	
	} else {
		php_error(E_WARNING, "SHM segment is not existing (bartlby running?)");	
		
		RETURN_FALSE;
	}
	
	
}

PHP_FUNCTION(bartlby_delete_server) {
	
	pval * bartlby_config;
	pval * server_id;
	
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*DeleteServer)(int, char*);
	
	struct service svc;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config,&server_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(bartlby_config);
	
	convert_to_long(server_id);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(DeleteServer,SOHandle, "DeleteServer");
	
	
	
	ret=DeleteServer(Z_LVAL_P(server_id),Z_STRVAL_P(bartlby_config));
	
	dlclose(SOHandle);
	RETURN_LONG(ret);
}
PHP_FUNCTION(bartlby_get_service_by_id) {
	pval * bartlby_config;
	
	pval * service_id;
	
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*GetServiceById)(int,struct service *, char *);
	
	struct service  svc;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config,&service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(bartlby_config);
	
	convert_to_long(service_id);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(GetServiceById,SOHandle, "GetServiceById");
	
	GetServiceById(Z_LVAL_P(service_id),&svc, Z_STRVAL_P(bartlby_config));
	if(svc.server_name == NULL) {
		RETURN_FALSE;	
	} else {
		
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_long(return_value, "server_id", svc.server_id);
		add_assoc_long(return_value, "last_state", svc.last_state);
		add_assoc_long(return_value, "last_state", svc.current_state);
		add_assoc_long(return_value, "client_port", svc.client_port);
				
		add_assoc_string(return_value, "new_server_text", svc.new_server_text, 1);
		add_assoc_string(return_value, "service_name", svc.service_name, 1);
		add_assoc_string(return_value, "server_name", svc.server_name, 1);
		add_assoc_string(return_value, "client_ip", svc.client_ip, 1);
		add_assoc_string(return_value, "plugin", svc.plugin, 1);
		add_assoc_string(return_value, "plugin_arguments", svc.plugin_arguments, 1);
		
		add_assoc_long(return_value, "check_interval", svc.check_interval);
		add_assoc_long(return_value, "last_check", svc.last_check);
		
		add_assoc_long(return_value, "hour_from", svc.hour_from);
		add_assoc_long(return_value, "min_from", svc.min_from);
		add_assoc_long(return_value, "hour_to", svc.hour_to);
		add_assoc_long(return_value, "min_to", svc.min_to);
		
		add_assoc_long(return_value, "notify_enabled", svc.notify_enabled);
		add_assoc_long(return_value, "last_notify_send", svc.last_notify_send);
		add_assoc_long(return_value, "flap_count", svc.flap_count);
		
		add_assoc_long(return_value, "service_type", svc.service_type);
		add_assoc_long(return_value, "service_passive_timeout", svc.service_passive_timeout);
		
		
		add_assoc_string(return_value, "service_var", svc.service_var, 1);
			
	}
	dlclose(SOHandle);
}

PHP_FUNCTION(bartlby_delete_service) {
		pval * bartlby_config;
	pval * service_id;
	
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*DeleteService)(int, char*);
	
	struct service svc;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config, &service_id) ==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(bartlby_config);
	
	convert_to_long(service_id);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(DeleteService,SOHandle, "DeleteService");
	
	
	
	ret=DeleteService(Z_LVAL_P(service_id),Z_STRVAL_P(bartlby_config));
	
	dlclose(SOHandle);
	RETURN_LONG(ret);
	
}

PHP_FUNCTION(bartlby_modify_service) {
	struct service svc;
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	int rtc;
	
	int (*UpdateService)(struct service *, char *);
	
	pval *service_id, * server_id , * bartlby_config, * plugin, * service_name , * plugin_arguments, * notify_enabled, * hour_from, * hour_to, *min_from, *min_to, *check_interval, *service_type, *service_passive_timeout, *service_var;
	
	/*
	svc->server_id, 
	svc->plugin, 
	svc->service_name,
	svc->plugin_arguments,
	svc->notify_enabled,
	1,
	SVC_TIME_FROM,
	SVC_TIME_TO,
	svc->check_interval,
	svc->service_type,
	svc->service_var,
	svc->service_passive_timeout
	*/
	
	if(ZEND_NUM_ARGS() != 15 || getParameters(ht, 15, &bartlby_config,&service_id,  &server_id, &plugin,&service_name,&plugin_arguments,&notify_enabled,&hour_from,&hour_to,&min_from,&min_to,&check_interval, &service_type,&service_var,&service_passive_timeout) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	convert_to_string(bartlby_config);
	convert_to_string(plugin);
	convert_to_string(service_name);
	convert_to_string(plugin_arguments);
	convert_to_long(notify_enabled);
	convert_to_long(service_id);

	convert_to_long(hour_from);
	convert_to_long(hour_to);
	convert_to_long(hour_from);
	convert_to_long(min_to);
	convert_to_long(check_interval);
	convert_to_long(service_type);
	convert_to_long(service_passive_timeout);
	convert_to_string(service_var);
	
	svc.service_id=Z_LVAL_P(service_id);
	sprintf(svc.plugin, "%s", Z_STRVAL_P(plugin));
	sprintf(svc.service_name, "%s", Z_STRVAL_P(service_name));
	sprintf(svc.plugin_arguments, "%s", Z_STRVAL_P(plugin_arguments));
	svc.notify_enabled=Z_LVAL_P(notify_enabled);
	
	svc.hour_from=Z_LVAL_P(hour_from);
	svc.hour_to=Z_LVAL_P(hour_to);
	svc.min_from=Z_LVAL_P(min_from);
	svc.min_to=Z_LVAL_P(min_to);
	
	svc.check_interval=Z_LVAL_P(check_interval);
	svc.service_type=Z_LVAL_P(service_type);
	svc.service_passive_timeout=Z_LVAL_P(service_passive_timeout);
	sprintf(svc.service_var, "%s", Z_STRVAL_P(service_var));
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(UpdateService,SOHandle, "UpdateService");
	
	rtc=UpdateService(&svc, Z_STRVAL_P(bartlby_config));
	
	dlclose(SOHandle);
	RETURN_LONG(rtc);
	
	
	
	
		
	
}

PHP_FUNCTION(bartlby_add_service) {
	struct service svc;
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	int rtc;
	
	int (*AddService)(struct service *, char *);
	
	pval * server_id , * bartlby_config, * plugin, * service_name , * plugin_arguments, * notify_enabled, * hour_from, * hour_to, *min_from, *min_to, *check_interval, *service_type, *service_passive_timeout, *service_var;
	
	/*
	svc->server_id, 
	svc->plugin, 
	svc->service_name,
	svc->plugin_arguments,
	svc->notify_enabled,
	1,
	SVC_TIME_FROM,
	SVC_TIME_TO,
	svc->check_interval,
	svc->service_type,
	svc->service_var,
	svc->service_passive_timeout
	*/
	
	if(ZEND_NUM_ARGS() != 14 || getParameters(ht, 14, &bartlby_config, &server_id, &plugin,&service_name,&plugin_arguments,&notify_enabled,&hour_from,&hour_to,&min_from,&min_to,&check_interval, &service_type,&service_var,&service_passive_timeout) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	convert_to_string(bartlby_config);
	convert_to_string(plugin);
	convert_to_string(service_name);
	convert_to_string(plugin_arguments);
	convert_to_long(notify_enabled);
	convert_to_long(hour_from);
	convert_to_long(hour_to);
	convert_to_long(hour_from);
	convert_to_long(min_to);
	convert_to_long(check_interval);
	convert_to_long(service_type);
	convert_to_long(service_passive_timeout);
	convert_to_string(service_var);
	
	sprintf(svc.plugin, "%s", Z_STRVAL_P(plugin));
	sprintf(svc.service_name, "%s", Z_STRVAL_P(service_name));
	sprintf(svc.plugin_arguments, "%s", Z_STRVAL_P(plugin_arguments));
	svc.notify_enabled=Z_LVAL_P(notify_enabled);
	
	svc.hour_from=Z_LVAL_P(hour_from);
	svc.hour_to=Z_LVAL_P(hour_to);
	svc.min_from=Z_LVAL_P(min_from);
	svc.min_to=Z_LVAL_P(min_to);
	
	svc.check_interval=Z_LVAL_P(check_interval);
	svc.service_type=Z_LVAL_P(service_type);
	svc.service_passive_timeout=Z_LVAL_P(service_passive_timeout);
	sprintf(svc.service_var, "%s", Z_STRVAL_P(service_var));
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(AddService,SOHandle, "AddService");
	
	rtc=AddService(&svc, Z_STRVAL_P(bartlby_config));
	
	dlclose(SOHandle);
	RETURN_LONG(rtc);
	
	
	
		
	
}
PHP_FUNCTION(bartlby_add_server) {
	
	pval * bartlby_config;
	pval * server_name;
	pval * server_ip;
	pval * server_port;
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*AddServer)(struct service *,char *);
	
	struct service svc;
	
	if (ZEND_NUM_ARGS() != 4 || getParameters(ht, 4, &bartlby_config,&server_name, &server_ip, &server_port)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(bartlby_config);
	convert_to_string(server_name);
	convert_to_string(server_ip);
	convert_to_long(server_port);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(AddServer,SOHandle, "AddServer");
	
	strcpy(svc.server_name, Z_STRVAL_P(server_name));
	svc.client_port=Z_LVAL_P(server_port);
	strcpy(svc.client_ip, Z_STRVAL_P(server_ip));
	
	ret=AddServer(&svc, Z_STRVAL_P(bartlby_config));
	
	dlclose(SOHandle);
	RETURN_LONG(ret);
}

PHP_FUNCTION(bartlby_modify_server) {
	
	pval * bartlby_config;
	pval * server_name;
	pval * server_ip;
	pval * server_port;
	pval * server_id;
	
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*ModifyServer)(struct service *,char *);
	
	struct service svc;
	
	if (ZEND_NUM_ARGS() != 5 || getParameters(ht, 5, &bartlby_config,&server_id, &server_name, &server_ip, &server_port)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(bartlby_config);
	convert_to_string(server_name);
	convert_to_string(server_ip);
	convert_to_long(server_port);
	convert_to_long(server_id);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(ModifyServer,SOHandle, "ModifyServer");
	
	strcpy(svc.server_name, Z_STRVAL_P(server_name));
	svc.client_port=Z_LVAL_P(server_port);
	svc.server_id=Z_LVAL_P(server_id);
	strcpy(svc.client_ip, Z_STRVAL_P(server_ip));
	
	ret=ModifyServer(&svc, Z_STRVAL_P(bartlby_config));
	
	dlclose(SOHandle);
	RETURN_LONG(ret);
}

PHP_FUNCTION(bartlby_get_server_by_id) {
	pval * bartlby_config;
	
	pval * server_id;
	
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*GetServerById)(int,struct service *, char *);
	
	struct service  svc;
	
	if (ZEND_NUM_ARGS() != 2 || getParameters(ht, 2, &bartlby_config,&server_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(bartlby_config);
	
	convert_to_long(server_id);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	
	
	LOAD_SYMBOL(GetServerById,SOHandle, "GetServerById");
	
	GetServerById(Z_LVAL_P(server_id),&svc, Z_STRVAL_P(bartlby_config));
	if(svc.server_name == NULL) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_string(return_value, "server_name", svc.server_name, 1);
		add_assoc_string(return_value, "server_ip", svc.client_ip, 1);
		add_assoc_long(return_value, "server_port",svc.client_port);
			
	}
	dlclose(SOHandle);
		
}

PHP_FUNCTION(bartlby_lib_info) {
	char * dlmsg;
	void * SOHandle;
	char * data_lib;
	pval * bartlby_config;
	
	char * (*GetAutor)();
	char * (*GetVersion)();
	char * (*GetName)();
	char * GetAutorStr;
	char * GetVersionStr;
	char * GetNameStr;
	
	if (ZEND_NUM_ARGS() != 1 || getParameters(ht, 1, &bartlby_config)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_string(bartlby_config);
	
	SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
	if(SOHandle == NULL) {
		php_error(E_WARNING, "bartlby SO error");	
		RETURN_FALSE;	
	}
	LOAD_SYMBOL(GetAutor,SOHandle, "GetAutor");
    	LOAD_SYMBOL(GetVersion,SOHandle, "GetVersion");
    	LOAD_SYMBOL(GetName,SOHandle, "GetName");
    	
	GetAutorStr=GetAutor();
    	GetVersionStr=GetVersion();
    	GetNameStr=GetName();
    	
    	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	add_assoc_string(return_value, "Autor", GetAutorStr, 1);
	add_assoc_string(return_value, "Version", GetVersionStr, 1);
	add_assoc_string(return_value, "Name", GetNameStr, 1);
	
	free(GetAutorStr);
	free(GetVersionStr);
	free(GetNameStr);
	dlclose(SOHandle);
    	
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
	
		
	
	
	
	bartlby_address=bartlby_get_shm(Z_STRVAL_P(bartlby_config)); 
	if(bartlby_address != NULL) {
		shm_hdr=(struct shm_header *)(void *)bartlby_address;
		svcmap=(struct service *)(void *)bartlby_address+sizeof(struct shm_header);
		
		
		if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
			php_error(E_WARNING, "Service id out of bounds");	
			RETURN_FALSE;	
		}
		add_assoc_long(return_value, "server_id", svcmap[Z_LVAL_P(bartlby_service_id)].server_id);
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
	
	bartlby_address=bartlby_get_shm(Z_STRVAL_P(bartlby_config));
	if(bartlby_address != NULL) {
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
		add_assoc_string(return_value, "notify_levels", wrkmap[Z_LVAL_P(bartlby_worker_id)].notify_levels,1);
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
