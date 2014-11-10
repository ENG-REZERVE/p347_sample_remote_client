
#ifndef JSON2PBInterface_HPP_
#define JSON2PBInterface_HPP_

#define CONV_INDENT_LEN		3

#include "pbjson.hpp"
#include <errno.h>
//#include <google/protobuf/message.h>

class JSON2PBInterface {
public:
	JSON2PBInterface() {};
	~JSON2PBInterface() {};

	int convertProtobufToJson(void* proto_class, std::string* json_result, bool do_readable_formatting) {
		if ((proto_class == NULL) || (json_result == NULL)) return -EINVAL;

		json_result->clear();

		pbjson::pb2json((const google::protobuf::Message*)proto_class, *json_result);

		int len = json_result->size();
		if ((len > 0) && (do_readable_formatting == true)) {
			int indent = 0;
			int pos = 0;
			while (pos < json_result->size()) {
				switch (json_result->at(pos)) {
					case '{':
					case '[': {
						indent += CONV_INDENT_LEN;
						json_result->insert(pos+1,"\n");
						pos+=2;
						json_result->insert(pos,indent,' ');
						pos+=indent;
					break; }
					case '}':
					case ']': {
						json_result->insert(pos,"\n");
						pos+=1;
						indent -= CONV_INDENT_LEN;
						json_result->insert(pos,indent,' ');
						pos = pos + indent + 1;
					break; }
					case ',': {
						json_result->insert(pos+1,"\n");
						pos+=2;
						json_result->insert(pos,indent,' ');
						pos+=indent;
					break; }
					case ':': {
						json_result->insert(pos+1," ");
						pos+=2;
					break; }
					default: {
						pos++;
					break; }
				}
			}
		}

		return 0;
	}

	int convertJsonToProtobuff(void* proto_class, std::string* json_source, std::string* err) {
		if ((proto_class == NULL) || (json_source == NULL) || (err == NULL)) return -EINVAL;
		if (json_source->size() <= 0) return -EINVAL;
		return pbjson::json2pb(*json_source, (google::protobuf::Message*)proto_class, *err);
	}

private:
};

#endif /* JSON2PBInterface_HPP_ */
