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

#ifndef PHP_BARTLBY_H
#define PHP_BARTLBY_H

extern zend_module_entry bartlby_module_entry;
#define phpext_bartlby_ptr &bartlby_module_entry

#ifdef PHP_WIN32
#define PHP_BARTLBY_API __declspec(dllexport)
#else
#define PHP_BARTLBY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <sys/types.h>
#include <sys/shm.h>

#define LOAD_SYMBOL(x,y,z) 	x=dlsym(y, z); \
    	if((dlmsg=dlerror()) != NULL) { \
        	php_error(E_WARNING, "dl error: %s", dlmsg);	\
		RETURN_FALSE;	\
    	}
    	

#define BARTLBY_VERSION "0.9.1"

PHP_MINIT_FUNCTION(bartlby);
PHP_MSHUTDOWN_FUNCTION(bartlby);
PHP_RINIT_FUNCTION(bartlby);
PHP_RSHUTDOWN_FUNCTION(bartlby);
PHP_MINFO_FUNCTION(bartlby);

PHP_FUNCTION(confirm_bartlby_compiled);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_worker);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_service);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_info);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_version);
PHP_FUNCTION(bartlby_config);
PHP_FUNCTION(bartlby_lib_info);
PHP_FUNCTION(bartlby_add_server);
PHP_FUNCTION(bartlby_delete_server);
PHP_FUNCTION(bartlby_modify_server);
PHP_FUNCTION(bartlby_get_server_by_id);



PHP_FUNCTION(bartlby_add_service);
PHP_FUNCTION(bartlby_delete_service);
PHP_FUNCTION(bartlby_modify_service);
PHP_FUNCTION(bartlby_get_service_by_id);


PHP_FUNCTION(bartlby_add_worker);
PHP_FUNCTION(bartlby_delete_worker);
PHP_FUNCTION(bartlby_modify_worker);
PHP_FUNCTION(bartlby_get_worker_by_id);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(bartlby)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(bartlby)
*/

/* In every utility function you add that needs to use variables 
   in php_bartlby_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as BARTLBY_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define BARTLBY_G(v) TSRMG(bartlby_globals_id, zend_bartlby_globals *, v)
#else
#define BARTLBY_G(v) (bartlby_globals.v)
#endif

#endif	/* PHP_BARTLBY_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */


struct shm_header {
	        int svccount;
	        int wrkcount;
	        int current_running;
		char  version[50];

};

struct service {
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
	
	/*Time stuff*/
	
	int hour_from;
	int min_from;
	int hour_to;
	int min_to;
	
	/*Notify things*/
	int notify_enabled;
	int last_notify_send;
	int flap_count;
	
	int service_active;
	
	char  service_var[2048];
	int service_type;
	int service_passive_timeout;
	
};

struct worker {
	char name[2048];
	char  mail[2048];
	char  icq[2048];
	char  services[2048];
	
	int worker_id;
	int active;
	int icq_notify;
	int mail_notify;
	int escalation_count;
	int escalation_time;
	char notify_levels[20];
	char t[500];

}sa;
