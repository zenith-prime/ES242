#include "test.h"
#include <stdio.h>
#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 */
void process_selection(int *b, int k, void *data)
{
}

void selecter(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data), int pos, int start)
{
    if (pos == k)
    {
        process_selection(b, k, data);
        return;
    }

    for (int i = start; i < n; i++)
    {
        b[pos] = a[i];
        selecter(a, n, k, b, data, process_selection, pos + 1, i + 1);
    }
}

void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
    //    b[0] = 2; b[1] = 1;
    //    process_selection(b, 2, data);
    //    b[0] = 2; b[1] = 6;
    //    process_selection(b, 2, data);
    //    b[0] = 2; b[1] = 5;
    //    process_selection(b, 2, data);
    //    b[0] = 1; b[1] = 6;
    //    process_selection(b, 2, data);
    //    b[0] = 1; b[1] = 5;
    //    process_selection(b, 2, data);
    //    b[0] = 6; b[1] = 5;
    //    process_selection(b, 2, data);

    selecter(a, n, k, b, data, process_selection, 0, 0);
}

/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.
 */
int is_word_in_dictionary(const char *word, const char *dictionary[], int nwords)
{
    for (int i = 0; i < nwords; i++)
    {
        if (strcmp(word, dictionary[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void generate_splits(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data))
{
    if (strlen(source) == 0)
    {
        process_split(buf, data);
        return;
    }

    for (int i = 1; i <= strlen(source); i++)
    {
        char word[100];
        strncpy(word, source, i);
        word[i] = '\0';

        if (is_word_in_dictionary(word, dictionary, nwords))
        {

            strcat(buf, word);
            strcat(buf, " ");

            char remaining[100];
            strcpy(remaining, source + i);

            generate_splits(remaining, dictionary, nwords, buf, data, process_split);

            buf[strlen(buf) - strlen(word) - 1] = '\0';
        }
    }
    // strcpy(buf, "art is toil");
    // process_split(buf, data);
    // strcpy(buf, "artist oil");
    // process_split(buf, data);
}

/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */

int previous_permutation(int a[], int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] <= a[i + 1])
    {
        i--;
    }

    if (i == -1)
    {
        // No previous permutation, return 0 or an appropriate value.
        return 0;
    }

    int j = n - 1;
    while (a[j] >= a[i])
    {
        j--;
    }

    // Swap a[i] and a[j]
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;

    // Reverse the subarray a[i+1, n-1]
    int start = i + 1;
    int end = n - 1;
    while (start < end)
    {
        temp = a[start];
        a[start] = a[end];
        a[end] = temp;
        start++;
        end--;
    }

    // Return 1 to indicate a previous permutation was found.
    return 1;
}

/* Write your tests here. Use the previous assignment for reference. */



/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
    int first;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    state_t *s = (state_t *)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
        case 0:
            if ((b[0] != 2) || (b[1] != 1)) {
                s->err = 1;
            }
            break;
        case 1:
            if ((b[0] != 2) || (b[1] != 6)) {
                s->err = 1;
            }
            break;
        case 2:
            if ((b[0] != 2) || (b[1] != 5)) {
                s->err = 1;
            }
            break;
        case 3:
            if ((b[0] != 1) || (b[1] != 6)) {
                s->err = 1;
            }
            break;
        case 4:
            if ((b[0] != 1) || (b[1] != 5)) {
                s->err = 1;
            }
            break;
        case 5:
            if ((b[0] != 6) || (b[1] != 5)) {
                s->err = 1;
            }
            break;
        default:
            s->err = 1;
    }
    ++(s->index);
}

void count_selections(int b[], int k, void *data)
{
    int *d = (int*)data;
    ++*d;
}

typedef struct {
    int b[100];
} selection_t;

void last_selection(int b[], int k, void *data)
{
    selection_t *s = (selection_t*)data;
    for (int i = 0; i < k; ++i) {
        s->b[i] = b[i];
    }
}

BEGIN_TEST(generate_selections) {
        int a[] = { 2, 1, 6, 5 };
        int aa[] = { 1, 5, 3, 0, 1, 12, 4, 3, 6, 6 };
        int bb[24];
        for (int i = 0; i < 24; ++i) {
            bb[i] = i;
        }
        int b[12];
        int c = 0;

        state_t s2165 = { .index = 0, .err = 1, .first = 1 };
        generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
        ASSERT(!s2165.err, "Failed on 2 1 6 5.");

        generate_selections(aa, 10, 5, b, &c, count_selections);
        ASSERT_EQ(c, 252, "Failed on 10C5.");

        selection_t s;
        generate_selections(aa, 10, 5, b, &s, last_selection);
        ASSERT_ARRAY_VALUES_EQ(s.b, 5, "Failed on last of 10C5.", 12, 4, 3, 6, 6);

        c = 0;
        generate_selections(bb, 24, 12, b, &c, count_selections);
        ASSERT_EQ(c, 2704156, "Failed on 24C12");

        generate_selections(bb, 24, 12, b, &s, last_selection);
        ASSERT_ARRAY_VALUES_EQ(s.b, 12, "Failed on last of 24C12", 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23);
    } END_TEST

void test_splits_art(char buf[], void *data)
{
    state_t *s = (state_t*)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
        case 0:
            if (strcmp(buf, "art is toil")) {
                s->err = 1;
            }
            break;
        case 1:
            if (strcmp(buf, "artist oil")) {
                s->err = 1;
            }
            break;
        default:
            s->err = 1;
    }
    ++(s->index);
}

void count_splits(char buf[], void *data)
{
    int *c = (int *)data;
    ++(*c);
}

BEGIN_TEST(generate_splits) {
        const char *dict[] = {
                "art",
                "artist",
                "is",
                "oil",
                "toil"
        };
        const char *dict_a[] = {
                "a",
                "aa",
                "aaa",
                "aaaa",
                "aaaaa",
                "aaaaaa",
                "aaaaaaa",
                "aaaaaaaa",
                "aaaaaaaaa",
                "aaaaaaaaaa"
        };
        state_t s = { .index = 0, .err = 1, .first = 1 };
        char buf[34000];
        char long_source[16000 + 1];
        for (int i = 0; i < 16000; ++i) {
            long_source[i] = 'a';
        }
        long_source[16000] = 0;
        int c;

        generate_splits("artistoil", dict, 5, buf, &s, test_splits_art);
        ASSERT(!s.err, "Failed on \'artistoil\'.");

        c = 0;
        generate_splits("aaaaaaaaaa", dict_a, 1, buf, &c, count_splits);
        ASSERT_EQ(c, 1, "Failed on \'aaaaaaaaaa\' with one split.");

        c = 0;
        generate_splits("aaaaaaaaaa", dict_a, 10, buf, &c, count_splits);
        ASSERT_EQ(c, 512, "Failed on \'aaaaaaaaaa\' with binary splits.");

        c = 0;
        generate_splits(long_source, dict_a, 1, buf, &c, count_splits);
        ASSERT_EQ(c, 1, "Failed on long source.");

        c = 0;
        generate_splits("aaaaaaaaaa", dict_a, 2, buf, &c, count_splits);
        ASSERT_EQ(c, 89, "Failed on Fibonacci split.");
    } END_TEST

BEGIN_TEST(previous_permutation) {
        int a[] = { 1, 5, 6, 2, 3, 4 };
        previous_permutation(a, 6);
        ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);

        int aa[] = { 1, 2, 3, 5, 4, 6 };
        previous_permutation(aa, 3); // 3 is correct.
        ASSERT_ARRAY_VALUES_EQ(aa, 3, "Failed on 1 2 3.", 1, 2, 3);

        previous_permutation(aa, 1);
        ASSERT_ARRAY_VALUES_EQ(aa, 6, "Failed on aa, 1.", 1, 2, 3, 5, 4, 6);

        int bb[] = { 1, 1, 1, 1 };
        previous_permutation(bb, 4);
        ASSERT_ARRAY_VALUES_EQ(bb, 4, "Failed on 4 1s.", 1, 1, 1, 1);

        previous_permutation(aa+3, 3);
        ASSERT_ARRAY_VALUES_EQ(aa, 6, "Failed on last part of aa.", 1, 2, 3, 4, 6, 5);
    } END_TEST

int main()
{
    run_tests((test_t[]) {
            TEST(generate_selections),
            TEST(generate_splits),
            TEST(previous_permutation),
            0
    });
    return 0;
}
