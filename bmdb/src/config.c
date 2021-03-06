/*
 * BitMeterOS
 * http://codebox.org.uk/bitmeterOS
 *
 * Copyright (c) 2011 Rob Dawson
 *
 * Licensed under the GNU General Public License
 * http://www.gnu.org/licenses/gpl.txt
 *
 * This file is part of BitMeterOS.
 *
 * BitMeterOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BitMeterOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BitMeterOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "common.h"
#include "bmdb.h"
#include <stdio.h>

/*
Displays a list of all the configuration values stored in the database.
*/

int doListConfig(FILE* file, int argc, char** argv){
	int rc;
	sqlite3_stmt *stmt;
	prepareSql(&stmt, "SELECT key, value FROM config");

	const unsigned char *key, *value;

    fprintf(file, INFO_DUMPING_CONFIG EOL);
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		key   = sqlite3_column_text(stmt, 0);
		value = sqlite3_column_text(stmt, 1);
		fprintf(file, "%s=%s" EOL, key, value);
	}
	sqlite3_finalize(stmt);

	if (rc != SQLITE_DONE){
		logMsg(LOG_ERR, " sqlite3_step returned %d in doConfig, %s", rc, getDbError());
		return FAIL;
	} else {
        return SUCCESS;
	}
}

int doSetConfig(FILE* file, int argc, char** argv){
    int status;
    if (argc == 2){
        status = setConfigTextValue(argv[0], argv[1]);
        if (status == SUCCESS){
        	printf("Config value '%s' set to '%s'\n", argv[0], argv[1]);
        } else {
	        printf("Error - failed to set config value.\n");
        }
    } else {
        printf("Error - expected 2 arguments, the name and value of the config parameter.\n");
        status = FAIL;
    }
    return status;
}

int doRmConfig(FILE* file, int argc, char** argv){
    int status;
    if (argc == 1){
        status = rmConfigValue(argv[0]);
        if (status == SUCCESS){
        	printf("Config value '%s' was removed\n", argv[0]);
        } else {
	        printf("Error - failed to remove config value.\n");
        }
    } else {
        printf("Error - expected 1 argument, the name of the config parameter to be removed.\n");
        status = FAIL;
    }
    return status;
}
