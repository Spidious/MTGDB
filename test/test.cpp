#include <gtest/gtest.h>
#include <mtgdb.h>

// Test API calls resolve to a basic result
TEST(API, RandomCall)
{
    // Create API and call random
    auto api = ScryfallAPI();
    api.BasicSearch("random");

    // Retrieve resultRandomCallTest
    auto res = api.GetResult();

    // Expect that the object type of the api call is "card"
    EXPECT_EQ(res["object"], "card");
}

// Test that the JSON result initializes
TEST(JSON, EmptyInit)
{
    // Expect successful initialization
    EXPECT_NO_THROW(new APIResult());
}

// Test that the JSON result can be initialized to the value of something
TEST(JSON, ManualInit)
{
    // create new result object
    const std::string simple_json = R"({"object":"custom"})"_json;
    APIResult res(simple_json);

    // Check the object initialized properly
    EXPECT_EQ(res["object"], "custom");
}


TEST(JSON, InitFromJsonObject)
{
    // Create the first res object
    const std::string simple_json = R"({"object":"custom"})"_json;
    const APIResult res(simple_json);

    // Copy value using constructor
    APIResult res2(res.dump());

    // Copy value using operator<<
    APIResult res3;
    res3 << res2.dump();

    // Copy value by assignment
    APIResult res4 = res3;

    // Check the final vs the original
    EXPECT_EQ(res4["object"], res["object"]);
}