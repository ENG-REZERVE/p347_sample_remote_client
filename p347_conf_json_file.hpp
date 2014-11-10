
#ifndef P347_CONF_JSONFILE_HPP_
#define P347_CONF_JSONFILE_HPP_

#include "p347_conf_abstract.hpp"
#include "json2pb_interface.hpp"

class p347JSONFileConf: public p347AbstractConfigurator, public JSON2PBInterface {
public:
	p347JSONFileConf();
	~p347JSONFileConf();

	virtual int loadConfigIdListForObjectType(unsigned int obj_type);
	virtual int loadConfig(unsigned int conf_id);
	virtual int loadBearing(unsigned int bearing_id, p347_conf::Bearing* dst);
	virtual int loadReducer(unsigned int reducer_id, p347_conf::Reducer* dst);
	//virtual int loadPorogDirData(unsigned int obj_type, unsigned int poi_idx, p347_conf::PorogDirData* dst) = 0;
	//virtual int loadEtalon(unsigned int typePoint,unsigned int obj_type, p347_conf::KdrEtalonData) = 0;

	int setFilenameConf(const char* name);
	int setFilenameConf(std::string name);
	int setFilenameConfIDList(const char* name);
	int setFilenameConfIDList(std::string name);
	int setFilenameBearings(const char* name);
	int setFilenameBearings(std::string name);
	int setFilenameReducers(const char* name);
	int setFilenameReducers(std::string name);
	int setFilenamePDD(const char* name);
	int setFilenamePDD(std::string name);
	int setFilenameEtalons(const char* name);
	int setFilenameEtalons(std::string name);

	void closeConfFile(void);
	void closeConfIdlistFile(void);
	void closeBearingsFile(void);
	void closeReducersFile(void);
	void closePDDFile(void);
	void closeEtalonsFile(void);

	void closeAllOpenedFiles(void);
private:
	std::string filename_conf;
	std::string filename_conf_idlist;
	std::string filename_bearings;
	std::string filename_reducers;
	std::string filename_pdd;
	std::string filename_etalons;

	FILE* file_conf;
	FILE* file_conf_idlist;
	FILE* file_bearings;
	FILE* file_reducers;
	FILE* file_pdd;
	FILE* file_etalons;

	p347_conf::ReducerList*		rlist;
	p347_conf::BearingList*	blist;
};

#endif
