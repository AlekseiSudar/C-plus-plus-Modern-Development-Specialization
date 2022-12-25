#include "test_runner.h"

#include <vector>
#include <algorithm>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    vector<Sentence<Token>> sentences;

    auto is_end = [](const Token& token){ return token.IsEndSentencePunctuation(); };
    auto it_begin_sent = tokens.begin(), it_end_sent = tokens.begin();
    while (it_end_sent != tokens.end()){
        it_end_sent = find_if(it_end_sent, tokens.end(), is_end);
        it_end_sent = find_if_not(it_end_sent, tokens.end(), is_end);

        sentences.push_back(Sentence<Token>());
        for (auto it = it_begin_sent; it != it_end_sent; it++)
            sentences.back().push_back(move(*it));
        it_begin_sent = it_end_sent;
    }

    return sentences;
}


struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
            vector<Sentence<TestToken>>({
                {{"Split"}, {"into"}, {"sentences"}, {"!"}}
            })
            );

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
            vector<Sentence<TestToken>>({
                {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
            })
            );

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
            vector<Sentence<TestToken>>({
                {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
                {{"Without"}, {"copies"}, {".", true}},
                })
                );
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}
