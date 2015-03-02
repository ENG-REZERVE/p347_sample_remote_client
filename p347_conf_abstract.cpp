/*
 * p347_conf_abstract.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: ukicompile
 */

#include "p347_conf_abstract.hpp"
#include "protobuf_convertors.h"

p347AbstractConfigurator::p347AbstractConfigurator() : UKI_LOG() {
	//do nothing
}

p347AbstractConfigurator::~p347AbstractConfigurator() {
	//do nothing
}

//---------------copy entire configuration
int p347AbstractConfigurator::copyAllConfTo(p347_conf::WorkConfiguration* dst) {
	if (dst == NULL) return -EINVAL;

	dst->CopyFrom(wconf);

	return 0;
}
/*
//---------------init
int p347AbstractConfigurator::getChannelManagerInitParamsPtr(channel_manager::ChannelManagerInitParams* par_ptr) {
	p347_conf::DeviceInitParams* dip = wconf.mutable_dip();
	if (dip == NULL) {
		par_ptr = NULL;
		return -ENOENT;
	}

	par_ptr = dip->mutable_cm_init();
	return 0;
}

int p347AbstractConfigurator::getMultiplexerInitParamsPtr(channel_manager::MultiplexerInitParams* par_ptr) {
	p347_conf::DeviceInitParams* dip = wconf.mutable_dip();
	if (dip == NULL) {
		par_ptr = NULL;
		return -ENOENT;
	}

	par_ptr = dip->mutable_mux_init();
	return 0;
}

int p347AbstractConfigurator::getDSPEmulInitParamsPtr(task_manager::DSPEmulInitParams* par_ptr) {
	p347_conf::DeviceInitParams* dip = wconf.mutable_dip();
	if (dip == NULL) {
		par_ptr = NULL;
		return -ENOENT;
	}

	par_ptr = dip->mutable_dspemul_init();
	return 0;
}

int p347AbstractConfigurator::copyChannelManagerInitParams(channel_manager::ChannelManagerInitParams* par_dst) {
	if (par_dst == NULL) return -EINVAL;

	channel_manager::ChannelManagerInitParams* cmip;
	int ret = getChannelManagerInitParamsPtr(cmip);
	if (ret == 0) {
		par_dst->CopyFrom(*cmip);
	}

	return ret;
}

int p347AbstractConfigurator::copyMultiplexerInitParams(channel_manager::MultiplexerInitParams* par_dst) {
	if (par_dst == NULL) return -EINVAL;

	channel_manager::MultiplexerInitParams* mip;
	int ret = getMultiplexerInitParamsPtr(mip);
	if (ret == 0) {
		par_dst->CopyFrom(*mip);
	}

	return ret;
}

int p347AbstractConfigurator::copyDSPEmulInitParams(task_manager::DSPEmulInitParams* par_dst) {
	if (par_dst == NULL) return -EINVAL;

	task_manager::DSPEmulInitParams* dip;
	int ret = getDSPEmulInitParamsPtr(dip);
	if (ret == 0) {
		par_dst->CopyFrom(*dip);
	}

	return ret;
}
*/
//---------------poi
int p347AbstractConfigurator::getObjectMapPtr(p347_conf::ObjectMap** map_ptr) {
	*map_ptr = wconf.mutable_object_map();

	return 0;
}

int p347AbstractConfigurator::getChainsPtrList(std::vector<p347_conf::DotChain*>* list_ptr) {
	if (list_ptr == NULL) return -EINVAL;
	list_ptr->clear();

	for (int i=0; i<wconf.dot_chains_size(); i++) {
		list_ptr->push_back(wconf.mutable_dot_chains(i));
	}

	return 0;
}

int p347AbstractConfigurator::getGroupsPtrListOfChain(unsigned int chain_number, std::vector<p347_conf::DotGroup*>* list_ptr) {
	if (list_ptr == NULL) return -EINVAL;
	if (chain_number >= wconf.dot_chains_size()) return -EINVAL;

	p347_conf::DotChain* cur_chain = wconf.mutable_dot_chains(chain_number);
	if (cur_chain == NULL) return -ENOENT;

	list_ptr->clear();

	for (int i=0; i<cur_chain->dot_groups_size(); i++) {
		list_ptr->push_back(cur_chain->mutable_dot_groups(i));
	}

	return 0;
}

int p347AbstractConfigurator::getDotsPtrListOfGroup(unsigned int chain_number, unsigned int group_number, std::vector<p347_conf::DotConf*>* list_ptr) {
	if (list_ptr == NULL) return -EINVAL;
	if (chain_number >= wconf.dot_chains_size()) return -EINVAL;

	p347_conf::DotChain* cur_chain = wconf.mutable_dot_chains(chain_number);
	if (cur_chain == NULL) return -ENOENT;

	if (group_number >= cur_chain->dot_groups_size()) return -EINVAL;

	p347_conf::DotGroup* cur_group = cur_chain->mutable_dot_groups(group_number);
	if (cur_group == NULL) return -ENOENT;

	list_ptr->clear();

	for (int i=0; i<cur_group->dot_configs_size(); i++) {
		list_ptr->push_back(cur_group->mutable_dot_configs(i));
	}

	return 0;
}

int p347AbstractConfigurator::getDotVibroPtrList(std::vector<p347_conf::DotVibro*>* list_ptr) {
	if (list_ptr == NULL) return -EINVAL;
	list_ptr->clear();

	int sz = wconf.dots_vibro_list_size();
	for (int i=0; i<sz; i++) {
		list_ptr->push_back(wconf.mutable_dots_vibro_list(i));
	}

	return 0;
}

int p347AbstractConfigurator::getDotVibroPtrByIdx(unsigned int dot_vibro_idx, p347_conf::DotVibro** dst_ptr) {
	int sz = wconf.dots_vibro_list_size();

	if (sz > 0)
		for (int i=0; i<sz; i++) {
			p347_conf::DotVibro* tmp_ptr = wconf.mutable_dots_vibro_list(i);
			if (tmp_ptr != NULL) {
				if (tmp_ptr->dot_idx() == dot_vibro_idx) {
					*dst_ptr = tmp_ptr;
					return 0;
				}
			}
		}

	return -ENOENT;
}

int p347AbstractConfigurator::getDotRotPtrByIdx(unsigned int dot_rot_idx, p347_conf::DotRot** dst_ptr) {
	int sz = wconf.dots_rot_list_size();

	if (sz > 0)
		for (int i=0; i<sz; i++) {
			p347_conf::DotRot* tmp_ptr = wconf.mutable_dots_rot_list(i);
			if (tmp_ptr != NULL) {
				if (tmp_ptr->dot_idx() == dot_rot_idx) {
					*dst_ptr = tmp_ptr;
					return 0;
				}
			}
		}

	return -ENOENT;
}

//---------------ckdr
int p347AbstractConfigurator::fillKdrINParamsForDot(p347_conf::DotVibro* dot, kdrINParams_t* dst) {
	if ((dot == NULL) || (dst == NULL)) return -EINVAL;

	p347_conf::KdrInParams* kip = dot->mutable_kdr_inparams();
	if (kip == NULL) return -ENOENT;

	int i=0;
	dst->typePoint = kip->typepoint();

	if (kip->has_drive_p())
		dst->p = kip->drive_p();
	if (kip->has_drive_z())
		dst->p = kip->drive_z();
	if (kip->has_drive_c())
		dst->p = kip->drive_c();

	dst->resursWorkBearing = kip->resursworkbearing();

	p347_conf::Bearing mainb;
	p347_conf::Bearing pairb;
	p347_conf::Reducer reducer;

	loadBearing(kip->main_bearing_id(),&mainb);
	loadBearing(kip->pair_bearing_id(),&pairb);
	loadReducer(kip->reducer_id(),&reducer);

	extractBearing(&mainb,&dst->bearing);
	extractBearing(&pairb,&dst->bearingPair);
	extractReducer(&reducer,&dst->reducer);

	dst->porogDirData = NULL;
	//loadPorogDirData(??? some id, allocated kdrPorogDirData_t);
	//TODO: convert to kdrPorogDirData_t

	int sz = kip->env_porog_size();
	for (i=0; i<sz; i++) {
		dst->envPorog[i] = kip->env_porog(i);
	}

	sz = kip->dir_porog_size();
	for (i=0; i<sz; i++) {
		dst->dirPorog[i] = kip->dir_porog(i);
	}

	dst->kurtosisPorog = kip->kurtosisporog();
	dst->ErrorFrot = kip->errorfrot();
	dst->MaxErrorFrot = kip->maxerrorfrot();
	dst->koeffRotation = kip->koeffrotation();

	dst->statis = NULL;
	dst->etalon = NULL;
	//TODO: loadEtalon(??? idx, kdrEtalonData_t *etalon);

	return 0;
}


