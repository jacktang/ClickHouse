#include <map>
#include <list>
#include <iostream>

#include <boost/assign/list_inserter.hpp>

#include <Poco/SharedPtr.h>

#include <DB/IO/ReadBufferFromIStream.h>
#include <DB/IO/WriteBufferFromOStream.h>

#include <DB/DataTypes/DataTypesNumberFixed.h>
#include <DB/DataTypes/DataTypeString.h>
#include <DB/DataTypes/DataTypeFixedString.h>
#include <DB/DataTypes/DataTypeDateTime.h>

#include <DB/DataStreams/TabSeparatedRowInputStream.h>
#include <DB/DataStreams/TabSeparatedBlockOutputStream.h>
#include <DB/DataStreams/BlockInputStreamFromRowInputStream.h>
#include <DB/DataStreams/copyData.h>

#include <DB/Storages/StorageLog.h>


using Poco::SharedPtr;


int main(int argc, char ** argv)
{
	try
	{
		DB::NamesAndTypesListPtr names_and_types_list = new DB::NamesAndTypesList;

		boost::assign::push_back(*names_and_types_list)
			("WatchID",				new DB::DataTypeUInt64)
			("JavaEnable",			new DB::DataTypeUInt8)
			("Title",				new DB::DataTypeString)
			("EventTime",			new DB::DataTypeDateTime)
			("CounterID",			new DB::DataTypeUInt32)
			("ClientIP",			new DB::DataTypeUInt32)
			("RegionID",			new DB::DataTypeUInt32)
			("UniqID",				new DB::DataTypeUInt64)
			("CounterClass",		new DB::DataTypeUInt8)
			("OS",					new DB::DataTypeUInt8)
			("UserAgent",			new DB::DataTypeUInt8)
			("URL",					new DB::DataTypeString)
			("Referer",				new DB::DataTypeString)
			("ResolutionWidth",		new DB::DataTypeUInt16)
			("ResolutionHeight",	new DB::DataTypeUInt16)
			("ResolutionDepth",		new DB::DataTypeUInt8)
			("FlashMajor",			new DB::DataTypeUInt8)
			("FlashMinor",			new DB::DataTypeUInt8)
			("FlashMinor2",			new DB::DataTypeString)
			("NetMajor",			new DB::DataTypeUInt8)
			("NetMinor",			new DB::DataTypeUInt8)
			("UserAgentMajor",		new DB::DataTypeUInt16)
			("UserAgentMinor",		new DB::DataTypeFixedString(2))
			("CookieEnable",		new DB::DataTypeUInt8)
			("JavascriptEnable",	new DB::DataTypeUInt8)
			("IsMobile",			new DB::DataTypeUInt8)
			("MobilePhone",			new DB::DataTypeUInt8)
			("MobilePhoneModel",	new DB::DataTypeString)
			("Params",				new DB::DataTypeString)
			("IPNetworkID",			new DB::DataTypeUInt32)
			("TraficSourceID",		new DB::DataTypeInt8)
			("SearchEngineID",		new DB::DataTypeUInt16)
			("SearchPhrase",		new DB::DataTypeString)
			("AdvEngineID",			new DB::DataTypeUInt8)
			("IsArtifical",			new DB::DataTypeUInt8)
			("WindowClientWidth",	new DB::DataTypeUInt16)
			("WindowClientHeight",	new DB::DataTypeUInt16)
			("ClientTimeZone",		new DB::DataTypeInt16)
			("ClientEventTime",		new DB::DataTypeDateTime)
			("SilverlightVersion1",	new DB::DataTypeUInt8)
			("SilverlightVersion2",	new DB::DataTypeUInt8)
			("SilverlightVersion3",	new DB::DataTypeUInt32)
			("SilverlightVersion4",	new DB::DataTypeUInt16)
			("PageCharset",			new DB::DataTypeString)
			("CodeVersion",			new DB::DataTypeUInt32)
			("IsLink",				new DB::DataTypeUInt8)
			("IsDownload",			new DB::DataTypeUInt8)
			("IsNotBounce",			new DB::DataTypeUInt8)
			("FUniqID",				new DB::DataTypeUInt64)
			("OriginalURL",			new DB::DataTypeString)
			("HID",					new DB::DataTypeUInt32)
			("IsOldCounter",		new DB::DataTypeUInt8)
			("IsEvent",				new DB::DataTypeUInt8)
			("IsParameter",			new DB::DataTypeUInt8)
			("DontCountHits",		new DB::DataTypeUInt8)
			("WithHash",			new DB::DataTypeUInt8)
		;

		SharedPtr<DB::DataTypes> data_types = new DB::DataTypes;

		for (DB::NamesAndTypesList::const_iterator it = names_and_types_list->begin(); it != names_and_types_list->end(); ++it)
			data_types->push_back(it->second);

		/// создаём описание, как читать данные из tab separated дампа

		DB::Block sample;
		for (DB::NamesAndTypesList::const_iterator it = names_and_types_list->begin(); it != names_and_types_list->end(); ++it)
		{
			DB::ColumnWithNameAndType elem;
			elem.name = it->first;
			elem.type = it->second;
			elem.column = elem.type->createColumn();
			sample.insert(elem);
		}

		/// читаем данные из строчного tsv файла и одновременно пишем в блочный tsv файл
		{
			DB::ReadBufferFromIStream in_buf(std::cin);
			DB::WriteBufferFromOStream out_buf(std::cout);

			DB::RowInputStreamPtr row_in = new DB::TabSeparatedRowInputStream(in_buf, sample);
			DB::BlockInputStreamFromRowInputStream in(row_in, sample);
			DB::TabSeparatedBlockOutputStream out(out_buf);
			DB::copyData(in, out);
		}
	}
	catch (const DB::Exception & e)
	{
		std::cerr << e.what() << ", " << e.displayText() << std::endl;
		return 1;
	}

	return 0;
}
