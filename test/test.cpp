#include <gtest/gtest.h>
#include <mtgdb.h>

const auto test_json_json = R"({"object": "empty"})"_json;
const auto test_json_string = R"({"object": "empty"})";

// Test that the JSON result initializes
TEST(JSON, EmptyInit)
{
    // Expect successful initialization
    EXPECT_NO_THROW(new Scryfall::APIResult());
}

// Test that the JSON result can be initialized to the value of something
TEST(JSON, ManualInit)
{
    // create new result object
    Scryfall::APIResult res(test_json_string);

    // Check the object initialized properly
    EXPECT_EQ(res["object"], "empty");
}

// Test the JSON init from output of another 
TEST(JSON, DataInit)
{
    // Create the first res object using string
    Scryfall::APIResult res(test_json_string);

    // Copy value using APIResult (implicit casting)
    Scryfall::APIResult res2(res);

    // Check the final vs the original
    EXPECT_EQ(res2["object"], "empty");
}

TEST(JSON, LeftShiftFromObj)
{
    // Create the first res object
    Scryfall::APIResult res(test_json_string);

    // Copy value using operator<<
    Scryfall::APIResult res2;
    res2 << res;

    // Check the final vs the original
    EXPECT_EQ(res2["object"], "empty");
}

TEST(JSON, Casting)
{
    // Cast APIResult from string and check
    auto res2 = static_cast<Scryfall::APIResult>(test_json_string);
    EXPECT_EQ(res2["object"], "empty") << "Could not cast from string";
}

// JSON object should fail when trying to create an object that doesn't have an object type
// Should come up with custom "object":"error" json result
TEST(JSON, InvalidJSON_Object)
{
    Scryfall::APIResult res;

    // Provide JSON with NO object parameter
    EXPECT_NO_THROW(res << R"({"code":"1234"})");
    EXPECT_EQ(res["object"], "error");

    // Provide JSON with an invalid object parameter
    EXPECT_NO_THROW(res << R"({"object":"bad_type","code":"1234"})");
    EXPECT_EQ(res["object"], "error");
}

// JSON object should fail when trying to create an object that doesn't have an object type
// Should contain information about the error
TEST(JSON, InvalidJSON_Exceptions)
{
    Scryfall::APIResult res;

    // Provide JSON with NO object parameter
    EXPECT_NO_THROW(res << R"({"code":"1234"})");
    EXPECT_LT(res.getStatus(), 0);
    EXPECT_NE(res.getException(), nullptr);

    // Provide JSON with an invalid object parameter
    EXPECT_NO_THROW(res << R"({"object":"bad_type","code":"1234"})");
    EXPECT_LT(res.getStatus(), 0);
    EXPECT_NE(res.getException(), nullptr);
}

// Test API call resolves to a basic result
TEST(API, RandomCall)
{
    // Create API and call random
    auto api = Scryfall::ScryfallAPI();
    api.BasicSearch("random");

    // Retrieve resultRandomCallTest
    auto res = api.GetResult();

    // Expect that the object type of the api call is "card"
    EXPECT_EQ(res["object"], "card") << R"(API call did not return "card" object)";
}

// Test BAD API call (card that will likely never exist)
TEST(API, InvalidCard)
{
    // Create a new API object
    auto api = Scryfall::ScryfallAPI();
    // Call API for random name
    api.CardSearch(Scryfall::ScryfallAPI::parse_query({{"name", ":", "32lk432lk4h324h34l24k"}}));
    auto res = api.GetResult();
    EXPECT_EQ(res["object"], "error");
    EXPECT_GE(res.getStatus(), 0);
    EXPECT_NE(res.getException(), nullptr);
}

int cli_handler(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}