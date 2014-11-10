#include "p347_conf_json_file.hpp"
#include <stdio.h>
#include <errno.h>

p347JSONFileConf::p347JSONFileConf() : p347AbstractConfigurator(){
	setPrefix("CC_JSONF");

	blist = NULL;
	rlist = NULL;

	file_conf = NULL;
	file_conf_idlist = NULL;
	file_bearings = NULL;
	file_reducers = NULL;
	file_pdd = NULL;
	file_etalons = NULL;
}

p347JSONFileConf::~p347JSONFileConf() {
	closeAllOpenedFiles();
}

int p347JSONFileConf::loadConfigIdListForObjectType(unsigned int obj_type) {
	logMessage(LOG_LEVEL_CRITICAL,"FUNCTION loadConfigIdListForObjectType NOT IMPLEMENTED!!!\n");
	return 0;
}

#define _load_contents(fil) \
	fseek(fil, 0, SEEK_END); \
	contents.resize(ftell(fil)); \
	rewind(fil); \
	fread(&contents[0], 1, contents.size(), fil); \
	fclose(fil); \
	fil = NULL

int p347JSONFileConf::loadConfig(unsigned int conf_id) {
	int ret = 0;

	if (filename_conf.size() <= 0) {
		logMessage(LOG_LEVEL_CRITICAL,"Invalid filename size!\n");
		return -EINVAL;
	}
	
	file_conf = fopen(filename_conf.c_str(),"r");
	if (file_conf == NULL) {
		ret = -errno;
		logMessage(LOG_LEVEL_CRITICAL,"file open failed with %d\n",ret);
		return ret;
	}
	std::string contents;

	_load_contents(file_conf);

	std::string errstr;
	p347_conf::WorkConfiguration wc_new;

	ret = pbjson::json2pb(contents, &wc_new, errstr);

	if (ret < 0) {
		logMessage(LOG_LEVEL_CRITICAL,"failed parse json to pb, ret %d [%s]\n",ret,errstr.c_str());
		return ret;
	}

	if (wc_new.conf_id() == conf_id) {
		logMessage(LOG_LEVEL_FULL,"found desired conf_id %d\n",conf_id);
		wconf.CopyFrom(wc_new);
		return 0;
	} else {
		logMessage(LOG_LEVEL_CRITICAL,"Cannot found conf id =%d\n",conf_id);
		return -ENOENT;
	}
}

int p347JSONFileConf::loadBearing(unsigned int bearing_id, p347_conf::Bearing* dst) {
	if (dst == NULL) return -EINVAL;
	if (filename_bearings.size() <= 0) return -ENOENT;

	if (blist == NULL) { //try to open
		file_bearings = fopen(filename_bearings.c_str(),"r");
		if (file_bearings == NULL) return -errno;

		std::string contents;
		fseek(file_bearings, 0, SEEK_END);
		contents.resize(ftell(file_bearings));
		rewind(file_bearings);
		fread(&contents[0], 1, contents.size(), file_bearings);
		fclose(file_bearings);
		file_bearings = NULL;

		std::string errstr;
		blist = new p347_conf::BearingList();
		int ret = pbjson::json2pb(contents, blist, errstr);
		if (ret < 0) {
			delete blist;
			blist = NULL;
			logMessage(LOG_LEVEL_CRITICAL,"failed parse json to pb, ret %d [%s]\n",ret,errstr.c_str());
			return ret;
		}
	}

	//then try to find needed from blist
	for (int i=0; i<blist->br_size(); i++) {
		p347_conf::Bearing* curb = blist->mutable_br(i);
		if (curb->bearing_idx() == bearing_id) {
			dst->CopyFrom(*curb);
			return 0;
		}
	}

	//TODO: another code
	return -ERANGE;
}

int p347JSONFileConf::loadReducer(unsigned int reducer_id, p347_conf::Reducer* dst) {
	if (dst == NULL) return -EINVAL;

	if (filename_reducers.size() <= 0) return -ENOENT;

	if (rlist == NULL) { //try to open
		file_reducers = fopen(filename_reducers.c_str(),"r");
		if (file_reducers == NULL) return -errno;

		std::string contents;
		fseek(file_reducers, 0, SEEK_END);
		contents.resize(ftell(file_reducers));
		rewind(file_reducers);
		fread(&contents[0], 1, contents.size(), file_reducers);
		fclose(file_reducers);
		file_reducers = NULL;

		std::string errstr;
		rlist = new p347_conf::ReducerList();
		int ret = pbjson::json2pb(contents, rlist, errstr);
		if (ret < 0) {
			delete rlist;
			rlist = NULL;
			logMessage(LOG_LEVEL_CRITICAL,"failed parse json to pb, ret %d [%s]\n",ret,errstr.c_str());
			return ret;
		}
	}

	//then try to find needed from rlist
	for (int i=0; i<rlist->rd_size(); i++) {
		p347_conf::Reducer* curr = rlist->mutable_rd(i);
		if (curr->reducer_idx() == reducer_id) {
			dst->CopyFrom(*curr);
			return 0;
		}
	}

	//TODO: another code
	return -ERANGE;
}

int p347JSONFileConf::setFilenameConf(const char* name) {
	if (name == NULL) return -EINVAL;
	if (strlen(name) <= 0) return -EINVAL;

	filename_conf.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameConf(std::string name) {
	if (name.size() <= 0) return -EINVAL;

	filename_conf.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameConfIDList(const char* name) {
	if (name == NULL) return -EINVAL;
	if (strlen(name) <= 0) return -EINVAL;

	filename_conf_idlist.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameConfIDList(std::string name) {
	if (name.size() <= 0) return -EINVAL;

	filename_conf_idlist.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameBearings(const char* name) {
	if (name == NULL) return -EINVAL;
	if (strlen(name) <= 0) return -EINVAL;

	filename_bearings.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameBearings(std::string name) {
	if (name.size() <= 0) return -EINVAL;

	filename_bearings.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameReducers(const char* name) {
	if (name == NULL) return -EINVAL;
	if (strlen(name) <= 0) return -EINVAL;

	filename_reducers.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameReducers(std::string name) {
	if (name.size() <= 0) return -EINVAL;

	filename_reducers.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenamePDD(const char* name) {
	if (name == NULL) return -EINVAL;
	if (strlen(name) <= 0) return -EINVAL;

	filename_pdd.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenamePDD(std::string name) {
	if (name.size() <= 0) return -EINVAL;

	filename_pdd.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameEtalons(const char* name) {
	if (name == NULL) return -EINVAL;
	if (strlen(name) <= 0) return -EINVAL;

	filename_etalons.assign(name);

	return 0;
}

int p347JSONFileConf::setFilenameEtalons(std::string name) {
	if (name.size() <= 0) return -EINVAL;

	filename_etalons.assign(name);

	return 0;
}

void p347JSONFileConf::closeConfFile(void) {
	if (file_conf != NULL) {
		fclose(file_conf);
		file_conf = NULL;
	}
}

void p347JSONFileConf::closeConfIdlistFile(void) {
	if (file_conf_idlist != NULL) {
		fclose(file_conf_idlist);
		file_conf_idlist = NULL;
	}
}

void p347JSONFileConf::closeBearingsFile(void) {
	if (file_bearings != NULL) {
		fclose(file_bearings);
		file_bearings = NULL;
	}
	if (blist != NULL) {
		delete blist;
		blist = NULL;
	}
}

void p347JSONFileConf::closeReducersFile(void) {
	if (file_reducers != NULL) {
		fclose(file_reducers);
		file_reducers = NULL;
	}
	if (rlist != NULL) {
		delete rlist;
		rlist = NULL;
	}
}

void p347JSONFileConf::closePDDFile(void) {
	if (file_pdd != NULL) {
		fclose(file_pdd);
		file_pdd = NULL;
	}
}

void p347JSONFileConf::closeEtalonsFile(void) {
	if (file_etalons != NULL) {
		fclose(file_etalons);
		file_etalons = NULL;
	}
}

void p347JSONFileConf::closeAllOpenedFiles(void) {
	closeConfFile();
	closeConfIdlistFile();
	closeBearingsFile();
	closeReducersFile();
	closePDDFile();
	closeEtalonsFile();
}

