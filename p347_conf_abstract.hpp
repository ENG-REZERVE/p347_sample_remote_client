/*
 * p347_conf_abstract.hpp
 *
 *  Created on: Oct 28, 2014
 *      Author: ukicompile
 */

#ifndef P347_CONF_ABSTRACT_HPP_
#define P347_CONF_ABSTRACT_HPP_

#include "p347_conf.pb.h"
#include "uki_log.hpp"
#include "ckdr.h"
#include <vector>
#include <errno.h>

class p347AbstractConfigurator : public UKI_LOG {
public:
	p347AbstractConfigurator();
	~p347AbstractConfigurator();

	virtual int loadConfigIdListForObjectType(unsigned int obj_type) = 0;
	virtual int loadConfig(unsigned int conf_id) = 0;
	virtual int loadBearing(unsigned int bearing_id, p347_conf::Bearing* dst) = 0;
	virtual int loadReducer(unsigned int bearing_id, p347_conf::Reducer* dst) = 0;
	//virtual int loadPorogDirData(unsigned int obj_type, unsigned int poi_idx, p347_conf::PorogDirData* dst) = 0;
	//virtual int loadEtalon(unsigned int typePoint,unsigned int obj_type, p347_conf::KdrEtalonData) = 0;
	//TODO: add writers

	//---------------copy entire configuration
	int copyAllConfTo(p347_conf::WorkConfiguration* dst);
	//---------------init
	/*
	int getChannelManagerInitParamsPtr(channel_manager::ChannelManagerInitParams* par_ptr);
	int getMultiplexerInitParamsPtr(channel_manager::MultiplexerInitParams* par_ptr);
	int getDSPEmulInitParamsPtr(task_manager::DSPEmulInitParams* par_ptr);
	int copyChannelManagerInitParams(channel_manager::ChannelManagerInitParams* par_dst);
	int copyMultiplexerInitParams(channel_manager::MultiplexerInitParams* par_dst);
	int copyDSPEmulInitParams(task_manager::DSPEmulInitParams* par_dst);
	//---------------poi
	*/
	int getObjectMapPtr(p347_conf::ObjectMap* map_ptr);
	int getChainsPtrList(std::vector<p347_conf::DotChain*>* list_ptr);
	int getGroupsPtrListOfChain(unsigned int chain_number, std::vector<p347_conf::DotGroup*>* list_ptr);
	int getDotsPtrListOfGroup(unsigned int chain_number, unsigned int group_number, std::vector<p347_conf::DotConf*>* list_ptr);
	int getDotVibroPtrList(std::vector<p347_conf::DotVibro*>* list_ptr);
	int getDotVibroPtrByIdx(unsigned int dot_vibro_idx, p347_conf::DotVibro* dst_ptr);
	int getDotRotPtrByIdx(unsigned int dot_rot_idx, p347_conf::DotRot* dst_ptr);
	//---------------ckdr
	int fillKdrINParamsForDot(p347_conf::DotVibro* dot, kdrINParams_t* dst);
protected:
	std::vector<int>				id_list;
	p347_conf::WorkConfiguration 	wconf;
};

#endif /* P347_CONF_ABSTRACT_HPP_ */
