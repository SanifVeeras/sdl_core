#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/ISchemaItem.hpp"
#include "SmartObjects/TSharedPtr.hpp"
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"
#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CObjectSchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"
#include "SmartObjects/TNumberSchemaItem.hpp"
#include "SmartObjects/TSchemaItemParameter.hpp"


#include <string>
#include <time.h>



namespace test { namespace components { namespace SmartObjects { namespace SmartObjectConvertionTimeTest {

    using namespace NsAppLink::NsJSONHandler::strings;
    
    class SmartObjectConvertionTimeTest : public ::testing::Test 
    {
        
    public:
        enum eTestType {
                APPLICATION_NOT_REGISTERED = 0,
                SUCCESS,
                TOO_MANY_PENDING_REQUESTS,
                REJECTED,
                INVALID_DATA,
                OUT_OF_MEMORY,
                ABORTED,
                USER_DISALLOWED,
                GENERIC_ERROR,
                DISALLOWED
            };
            
    protected:

        double getConvertionTimeToJsonV2Format(const NsAppLink::NsSmartObjects::CSmartObject & srcObj
            , std::string & jsonString) 
        {
            timespec convertionStartTime;
            timespec convertionEndTime;
            double convertionTime;
            
            clock_gettime(CLOCK_REALTIME, &convertionStartTime);
        
            // SmartObjects --> JSON
            NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(srcObj, jsonString);
            
            clock_gettime(CLOCK_REALTIME, &convertionEndTime);
            
            convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec) 
                + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;
            
            return convertionTime;
        }

        double getConvertionTimeFromJsonV2Format(std::string & jsonString
            ,NsAppLink::NsSmartObjects::CSmartObject & dstObj) 
        {
            timespec convertionStartTime;
            timespec convertionEndTime;
            double convertionTime;
            
            clock_gettime(CLOCK_REALTIME, &convertionStartTime);
        
            // JSON --> SmartObjects
            NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::
                fromString<std::string, std::string>(jsonString, dstObj, "some function", "request", 13);
            
            clock_gettime(CLOCK_REALTIME, &convertionEndTime);
            
            convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec) 
                + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;
            
            return convertionTime;
        }

        double getConvertionTimeToJsonV1Format(const NsAppLink::NsSmartObjects::CSmartObject & srcObj
            , std::string & jsonString) 
        {
            timespec convertionStartTime;
            timespec convertionEndTime;
            double convertionTime;
            
            clock_gettime(CLOCK_REALTIME, &convertionStartTime);
        
            // SmartObjects --> JSON
            NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::toString(srcObj, jsonString);
            
            clock_gettime(CLOCK_REALTIME, &convertionEndTime);
            
            convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec) 
                + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;
            
            return convertionTime;
        }

        double getConvertionTimeFromJsonV1Format(std::string & jsonString
            ,NsAppLink::NsSmartObjects::CSmartObject & dstObj) 
        {
            timespec convertionStartTime;
            timespec convertionEndTime;
            double convertionTime;
            
            clock_gettime(CLOCK_REALTIME, &convertionStartTime);
        
            // JSON --> SmartObjects
            NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::fromString(jsonString, dstObj);
            
            clock_gettime(CLOCK_REALTIME, &convertionEndTime);
            
            convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec) 
                + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;
            
            return convertionTime;
        }
        
        void calculateConvertionTime(const NsAppLink::NsSmartObjects::CSmartObject & srcObj
            ,NsAppLink::NsSmartObjects::CSmartObject & dstObj)
        {
            std::string jsonString;
            double convertionToTime = 0.0;
            double convertionFromTime = 0.0;
            int cycles = 10;
            
            for(int i = 0; i < cycles; i++)
            {
                convertionToTime += getConvertionTimeToJsonV1Format(srcObj, jsonString);
                convertionFromTime += getConvertionTimeFromJsonV1Format(jsonString, dstObj);
            }
            printf("\nFormat V1. Convertion TO time = %.8f, Convertion FROM time = %.8f\n"
                    , (convertionToTime / (double)cycles)
                    , (convertionFromTime / (double)cycles));
            
            convertionToTime = 0.0;
            convertionFromTime = 0.0;
            for(int i = 0; i < cycles; i++)
            {
                convertionToTime += getConvertionTimeToJsonV2Format(srcObj, jsonString);
                convertionFromTime += getConvertionTimeFromJsonV2Format(jsonString, dstObj);
            }
            printf("Format V2. Convertion TO time = %.8f, Convertion FROM time = %.8f\n\n"
                    , (convertionToTime / (double)cycles)
                    , (convertionFromTime / (double)cycles));
        }
        
        void calculateConvertionTimeWithJsonStringOutput(const NsAppLink::NsSmartObjects::CSmartObject & srcObj
            ,NsAppLink::NsSmartObjects::CSmartObject & dstObj)
        {
            std::string jsonString;
            double convertionToTime;
            double convertionFromTime;
            
            convertionToTime = getConvertionTimeToJsonV1Format(srcObj, jsonString);
            convertionFromTime = getConvertionTimeFromJsonV1Format(jsonString, dstObj);
            printf("\nJSON string V1 = %s", jsonString.c_str());
            printf("\nFormat V1. Convertion TO time = %.8f, Convertion FROM time = %.8f\n"
                    , (convertionToTime)
                    , (convertionFromTime));
            
            convertionToTime = getConvertionTimeToJsonV2Format(srcObj, jsonString);
            convertionFromTime = getConvertionTimeFromJsonV2Format(jsonString, dstObj);
            printf("\nJSON string V2 = %s", jsonString.c_str());
            printf("\nFormat V2. Convertion TO time = %.8f, Convertion FROM time = %.8f\n"
                    , (convertionToTime)
                    , (convertionFromTime));
        }
        
        //Create SmartObjectSchema for test object
        NsAppLink::NsSmartObjects::CSmartSchema initObjectSchema(void)
        {
            std::set<eTestType> resultCode_allowedEnumSubsetValues;
            resultCode_allowedEnumSubsetValues.insert(APPLICATION_NOT_REGISTERED);
            resultCode_allowedEnumSubsetValues.insert(SUCCESS);
            resultCode_allowedEnumSubsetValues.insert(TOO_MANY_PENDING_REQUESTS);
            resultCode_allowedEnumSubsetValues.insert(REJECTED);
            resultCode_allowedEnumSubsetValues.insert(INVALID_DATA);
            resultCode_allowedEnumSubsetValues.insert(OUT_OF_MEMORY);
            resultCode_allowedEnumSubsetValues.insert(ABORTED);
            resultCode_allowedEnumSubsetValues.insert(USER_DISALLOWED);
            resultCode_allowedEnumSubsetValues.insert(GENERIC_ERROR);
            resultCode_allowedEnumSubsetValues.insert(DISALLOWED);

            NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> success_SchemaItem = 
                NsAppLink::NsSmartObjects::CBoolSchemaItem::create(NsAppLink::NsSmartObjects::TSchemaItemParameter<bool>());

            NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> resultCode_SchemaItem = 
                NsAppLink::NsSmartObjects::TEnumSchemaItem<eTestType>::create(resultCode_allowedEnumSubsetValues
                    , NsAppLink::NsSmartObjects::TSchemaItemParameter<eTestType>());

            NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> info_SchemaItem = 
                NsAppLink::NsSmartObjects::CStringSchemaItem::create(NsAppLink::NsSmartObjects::TSchemaItemParameter<size_t>(1000)
                    , NsAppLink::NsSmartObjects::TSchemaItemParameter<std::string>());

            NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> tryAgainTime_SchemaItem = 
                NsAppLink::NsSmartObjects::TNumberSchemaItem<int>::create(NsAppLink::NsSmartObjects::TSchemaItemParameter<int>(0)
                    , NsAppLink::NsSmartObjects::TSchemaItemParameter<int>(2000000000)
                    , NsAppLink::NsSmartObjects::TSchemaItemParameter<int>());

            std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember> schemaMembersMap;

            schemaMembersMap["success"] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(success_SchemaItem, true);
            schemaMembersMap["resultCode"] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
            schemaMembersMap["info"] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(info_SchemaItem, false);
            schemaMembersMap["tryAgainTime"] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(tryAgainTime_SchemaItem, true);

            std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember> paramsMembersMap;
            paramsMembersMap[NsAppLink::NsJSONHandler::strings::S_FUNCTION_ID] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::TEnumSchemaItem<eTestType>::create(resultCode_allowedEnumSubsetValues), true);
            paramsMembersMap[NsAppLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::TEnumSchemaItem<eTestType>::create(resultCode_allowedEnumSubsetValues), true);
            paramsMembersMap[NsAppLink::NsJSONHandler::strings::S_CORRELATION_ID] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::TNumberSchemaItem<int>::create(), true);
            paramsMembersMap[NsAppLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::TNumberSchemaItem<int>::create(1, 2), true);
            paramsMembersMap[NsAppLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::TNumberSchemaItem<int>::create(), true);

            std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember> rootMembersMap;
            rootMembersMap[NsAppLink::NsJSONHandler::strings::S_MSG_PARAMS] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::CObjectSchemaItem::create(schemaMembersMap), true);
            rootMembersMap[NsAppLink::NsJSONHandler::strings::S_PARAMS] = NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember(NsAppLink::NsSmartObjects::CObjectSchemaItem::create(paramsMembersMap), true);

            return NsAppLink::NsSmartObjects::CSmartSchema(NsAppLink::NsSmartObjects::CObjectSchemaItem::create(rootMembersMap));
        }
    };
    
    TEST_F(SmartObjectConvertionTimeTest, test_int_object_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj;
       
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["value"] = 5;
        
        printf("\n INT value.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }

    TEST_F(SmartObjectConvertionTimeTest, test_double_object_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj;
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["value1"] = 3.1415926;
        srcObj[S_MSG_PARAMS]["value2"] = 32.6;
        srcObj[S_MSG_PARAMS]["value3"] = 33.945;
        srcObj[S_MSG_PARAMS]["value4"] = -12.5487698;
        srcObj[S_MSG_PARAMS]["value5"] = 0.61287346;
        
        printf("\n Double value.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }
    
    TEST_F(SmartObjectConvertionTimeTest, test_some_object_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj;
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["appId"] = "APP ID";
        srcObj[S_MSG_PARAMS]["appName"] = "APP NAME";
        srcObj[S_MSG_PARAMS]["appType"][0] = "SYSTEM";
        srcObj[S_MSG_PARAMS]["appType"][1] = "COMMUNICATION";
        srcObj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = "RU-RU";
        srcObj[S_MSG_PARAMS]["isMediaApplication"] = true;
        srcObj[S_MSG_PARAMS]["languageDesired"] = "EN-US";
        srcObj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
        srcObj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
        srcObj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
        srcObj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
        srcObj[S_MSG_PARAMS]["ttsName"][0]["type"] = "TEXT";
        srcObj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
        srcObj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
        srcObj[S_MSG_PARAMS]["null"] = NsAppLink::NsSmartObjects::CSmartObject();
        srcObj[S_MSG_PARAMS]["double"] = -0.1234;
        
        printf("\n Random object.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }
    
    TEST_F(SmartObjectConvertionTimeTest, test_map_object_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj, mapObj, innerObj;
       
// First iteration        
        mapObj["request"]["name"] = "My Request";
        mapObj["request"]["id"] = 123;
        mapObj["response"]["name"] = "My Response";
        mapObj["response"]["id"] = 456;
        mapObj["we"]["need"]["to"]["go"]["deeper"] = true;
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["value"] = mapObj;
        
        printf("\n MAP object.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);

// Second iteration        
        innerObj = mapObj;
        mapObj["request"]["value"] = innerObj;
        mapObj["response"]["value"] = innerObj;
        mapObj["we"]["need"]["to"]["go"]["deeper"]["value"] = innerObj;
        srcObj[S_MSG_PARAMS]["value"] = mapObj;
        
        printf("\n Complex MAP object.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
        
// Third iteration        
        innerObj = mapObj;
        mapObj["request"]["value"] = innerObj;
        mapObj["response"]["value"] = innerObj;
        mapObj["we"]["need"]["to"]["go"]["deeper"]["value"] = innerObj;
        srcObj[S_MSG_PARAMS]["value"] = mapObj;
        
        printf("\n Very Complex MAP object.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
        
// Last iteration
        innerObj = mapObj;
        mapObj["request"]["value"] = innerObj;
        mapObj["response"]["value"] = innerObj;
        mapObj["we"]["need"]["to"]["go"]["deeper"]["value"] = innerObj;
        srcObj[S_MSG_PARAMS]["value"] = mapObj;
        
        printf("\n Very Very Complex MAP object.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }

    TEST_F(SmartObjectConvertionTimeTest, test_array_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj, arrayObj, innerObj;
        int arraySize = 10;
       
// First iteration        
        for(int i = 0; i < arraySize; i++)
        {
            arrayObj[i] = rand();
        }
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["array"] = arrayObj;

        printf("\n Array object [%d].\n", arraySize);
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
        
// Second iteration
        printf("\n Array object [%d x %d].\n", arraySize, arraySize);
        innerObj = arrayObj;
        for(int i = 0; i < arraySize; i++)
        {
            arrayObj[i] = innerObj;
        }
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["array"] = arrayObj;
        
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);

// Third iteration
        printf("\n Array object [%d x %d x %d].\n"
            , arraySize, arraySize, arraySize);
        innerObj = arrayObj;
        for(int i = 0; i < arraySize; i++)
        {
            arrayObj[i] = innerObj;
        }
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["array"] = arrayObj;
        
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);

// Fourth iteration        
        printf("\n Array object [%d x %d x %d x %d].\n"
            , arraySize, arraySize, arraySize, arraySize);
        innerObj = arrayObj;
        for(int i = 0; i < arraySize; i++)
        {
            arrayObj[i] = innerObj;
        }
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["array"] = arrayObj;
        
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);

// Last iteration
        printf("\n Array object [%d x %d x %d x %d x %d].\n"
            , arraySize, arraySize, arraySize, arraySize, arraySize);
        innerObj = arrayObj;
        for(int i = 0; i < arraySize; i++)
        {
            arrayObj[i] = innerObj;
        }
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["array"] = arrayObj;
        
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }
    
    TEST_F(SmartObjectConvertionTimeTest, test_object_with_enum_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj;
        NsAppLink::NsSmartObjects::CSmartSchema schema = initObjectSchema();
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["success"] = true;
        srcObj[S_MSG_PARAMS]["resultCode"] = 2;
        srcObj[S_MSG_PARAMS]["info"] = "Some string";
        srcObj[S_MSG_PARAMS]["tryAgainTime"] = 322;
        srcObj.setSchema(schema);
        
        printf("\n Object with enum.\n");
        calculateConvertionTime(srcObj, dstObj);
        
        dstObj.setSchema(schema);
        dstObj.getSchema().applySchema(dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }    

        TEST_F(SmartObjectConvertionTimeTest, test_object_without_enum_convertion)
    {
        NsAppLink::NsSmartObjects::CSmartObject srcObj, dstObj;
        
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
        srcObj[S_MSG_PARAMS]["success"] = true;
        srcObj[S_MSG_PARAMS]["resultCode"] = 2;
        srcObj[S_MSG_PARAMS]["info"] = "Some string";
        srcObj[S_MSG_PARAMS]["tryAgainTime"] = 322;
        
        printf("\n Object without enum.\n");
        calculateConvertionTime(srcObj, dstObj);
        EXPECT_TRUE(srcObj == dstObj);
    }    

}}}}

namespace NsAppLink { namespace NsSmartObjects {
    template<>
    const std::map<test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType
        ,std::string> & TEnumSchemaItem<test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType
            ,std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::APPLICATION_NOT_REGISTERED, "APPLICATION_NOT_REGISTERED"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::SUCCESS, "SUCCESS"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::TOO_MANY_PENDING_REQUESTS, "TOO_MANY_PENDING_REQUESTS"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::REJECTED, "REJECTED"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::INVALID_DATA, "INVALID_DATA"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::OUT_OF_MEMORY, "OUT_OF_MEMORY"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::ABORTED, "ABORTED"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::USER_DISALLOWED, "USER_DISALLOWED"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::GENERIC_ERROR, "GENERIC_ERROR"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::SmartObjectConvertionTimeTest::eTestType::DISALLOWED, "DISALLOWED"));
            
            isInitialized = true;
        }
        return enumStringRepresentationMap;
    }
}}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}