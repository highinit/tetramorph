#include "loadconf.h"

void LoadConf::doLoadRequired(json_t *root, const std::vector<std::string> &_required, ConfigParams &_params, 
								const std::string &_config_file) {
	
	for (int i = 0; i<_required.size(); i++) {

		json_t *j_param = json_object_get(root, _required[i].c_str());
		if (!json_is_string(j_param)) {
			json_decref(root);
			throw std::string("LoadConf::load ") + _config_file + " : " + _required[i] + " not set";
		}
		
		_params[ _required[i] ] = json_string_value(j_param);
	}
}

void LoadConf::doLoadOptional(json_t *root, const std::vector<std::string> &_optional, ConfigParams &_params) {
	
	for (int i = 0; i<_optional.size(); i++) {

		json_t *j_param = json_object_get(root, _optional[i].c_str());
		if (json_is_string(j_param)) {
			_params[ _optional[i] ] = json_string_value(j_param);
		}
	}
}

ConfigParams LoadConf::load (const std::string &_config_file, const std::vector<std::string> &_required) {
	
	ConfigParams ret;
	FILE *f = fopen(_config_file.c_str(), "r");
	
	json_t *root = NULL;
	json_error_t error;
	
	if (f == NULL) {
		
		throw std::string("LoadConf::load could not open file ") + _config_file;
		
	} else {
	
		root = json_loadf(f, 0, &error);
		fclose(f);
	}

	if (root == NULL)
		throw std::string("LoadConf::load could not parse file ") + _config_file;

	doLoadRequired(root, _required, ret, _config_file);
	
	json_decref(root);
	return ret;
}

ConfigParams LoadConf::load (const std::string &_config_file,
														const std::vector<std::string> &_required,
														const std::vector<std::string> &_optional) {

	ConfigParams ret;
	FILE *f = fopen(_config_file.c_str(), "r");
	
	json_t *root = NULL;
	json_error_t error;
	
	if (f == NULL) {
		
		throw std::string("LoadConf::load could not open file ") + _config_file;
		
	} else {
	
		root = json_loadf(f, 0, &error);
		fclose(f);
	}

	if (root == NULL)
		throw std::string("LoadConf::load could not parse file ") + _config_file;
	
	doLoadRequired(root, _required, ret, _config_file);
	doLoadOptional(root, _optional, ret);
	
	json_decref(root);
	return ret;
}
