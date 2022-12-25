void TestComputeMedianAge() {
    vector<Person> range;;
    ASSERT_EQUAL(ComputeMedianAge(range.begin(), range.end()), 0);

    range = {{ 10, Gender::MALE, true },
             { 11, Gender::MALE, true },
             { 12, Gender::MALE, true },
             { 13, Gender::MALE, true },
             { 14, Gender::MALE, true },
             { 15, Gender::MALE, true },
             { 16, Gender::MALE, true },
             { 17, Gender::MALE, true }};
    ASSERT_EQUAL(ComputeMedianAge(range.begin(), range.end()), 14);
    range.push_back({ 18, Gender::MALE, true });
    ASSERT_EQUAL(ComputeMedianAge(range.begin(), range.end()), 14);
}

void TestReadPersons() {
    istringstream input("2\n10 1 0\n15 0 1");

    vector<Person> persons = ReadPersons(input);
    vector<Person> expected = {{10, Gender::MALE, false}, {15, Gender::FEMALE, true}};
    ASSERT_EQUAL(persons, expected);
}

void TestPrintStats() {
    vector<Person> persons = {{ 10,   Gender::MALE,  true },
                              { 11, Gender::FEMALE,  true },
                              { 12,   Gender::MALE, false },
                              { 13,   Gender::MALE,  true },
                              { 14, Gender::FEMALE,  true },
                              { 15, Gender::FEMALE, false },
                              { 16,   Gender::MALE, false },
                              { 17,   Gender::MALE,  true }};
    AgeStats stats = ComputeStats(persons);
    ostringstream out;
    PrintStats(stats, out);
    string result = out.str();

    string expected = "Median age = 14\n";
    expected += "Median age for females = 14\n";
    expected += "Median age for males = 13\n";
    expected += "Median age for employed females = 14\n";
    expected += "Median age for unemployed females = 15\n";
    expected += "Median age for employed males = 13\n";
    expected += "Median age for unemployed males = 16\n";

    ASSERT_EQUAL(result, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestComputeMedianAge);
    RUN_TEST(tr, TestReadPersons);
    RUN_TEST(tr, TestPrintStats);
    return 0;
}
