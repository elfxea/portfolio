'''
    Sample #1: Correcting mistakes in the text using Levinstein distances and dictionary

    Sample text written in Russian located at ./input/sample_text.txt
    Sample dictionary for this program located at ./input/sample_dictionary.txt

    Input dictionary format:
        word frequency
            string word -- Any word. Not necessarily from the dictionary.
            int frequency -- Word frequency. Used to replace words (algorithm chooses word to replace with max frequency)

    Sample output available at ./output/sample_text.txt
'''


class Text:
    def __init__(self):
        self.INFINITE = 1000000000  # "Infinity" for Levinstein distance
        self.restricted = [  # An array of symbols to be skipped
            '!',
            '.',
            '?',
            ',',
            '—',
            ';',
            ':',
            '«',
            '»',
            '(',
            ')',
            '…'
        ]
        self.input_data = ''  # Normalized input data (excluding self.restricted symblos)
        self.input_data_length = 0

        self.input_dict = []  # Two-dimensions dictionary array (array of pairs ['string': frequency rate]
        self.normalized_dict = []  # Dictionary array without frequency rates

        self.unique_forms = []  # Unique words in text
        self.unique_forms_in_dict = []  # Unique words included in the dictionary
        self.unique_forms_length = 0  # Counter for unique forms (I don't like to use len() function everywhere
        #                               to save time. Generally, it takes O(N) operations, that is not good :( )
        self.unique_forms_in_dict_length = 0  # Counter for unique forms in dictionary

        self.potential_mistakes = []  # An array with potential mistakes (words not included in the dictionary)
        self.mistakes_distance = {}  # Levinstein distances for potential mistakes ('word': distance)
        self.mistakes_corrected = {}  # A dictionary for corrected mistakes (Levistein's distance < 3.
        #                               Format: 'word': 'corrected word)
        self.mistakes_corrected_oftenity = {}  # An auxillary dictionary to choose correction with most frequently rate

    def push(self, row, flag):  # Pushes normalized data to corresponding arrays
        if flag == 'text':
            for x in self.restricted:  # Normalizing input data
                row = row.replace(x, '')
            row = row.replace('  ', ' ')
            self.input_data += row
        elif flag == 'dict':  # Saving dictionary
            self.input_dict.append(tmp)
            self.normalized_dict.append(tmp[0])
        elif row == '' and flag == 'done':  # Splitting input by space. Calling function with flag='done' argument
            #                                 should be the last function call (to avoid incorrect input data)
            self.input_data = self.input_data.split()
            self.input_data_length = len(self.input_data)
        return

    def findWordForms(self):  # Finds unique words in text
        for word in self.input_data:
            if word not in self.unique_forms:
                self.unique_forms.append(word)
                self.unique_forms_length += 1
        return

    def findNeedle(self):  # Finds unique words included in the dictionary and potential mistakes
        for word in self.unique_forms:
            if word in self.normalized_dict:
                self.unique_forms_in_dict.append(word)
                self.unique_forms_in_dict_length += 1
            elif word not in self.potential_mistakes:  # If word is not in Unique forms, word is a potential mistake
                #                                        with "infinite" Levinstein distance
                self.potential_mistakes.append(word)
                self.mistakes_distance[word] = self.INFINITE
        return

    def distance(self, word, dictionary_word):  # Calculate Levinstein distance between word and dictionary word
        n, m = len(word), len(dictionary_word)

        current_row = range(n + 1)  # This variable used to store only rows, not the entire matrix
        for i in range(1, m + 1):
            previous_row, current_row = current_row, [i] + [0] * n
            for j in range(1, n + 1):
                add, delete, change = previous_row[j] + 1, current_row[j - 1] + 1, previous_row[j - 1]
                if word[j - 1] != dictionary_word[i - 1]:
                    change += 1
                current_row[j] = min(add, delete, change)
        return current_row[n]

    def findDistance(self):  # Finds Levinstein distances for all of the mistakes
        for word in self.potential_mistakes:
            j = 0
            for dictionary_word in self.normalized_dict:
                tmp = self.distance(word, dictionary_word)
                if self.mistakes_distance[word] > tmp:
                    self.mistakes_distance[word] = tmp
                    if tmp < 3 and word not in self.mistakes_corrected:  # if word is not corrected yet
                        self.mistakes_corrected[word] = dictionary_word
                        self.mistakes_corrected_oftenity[word] = int(self.input_dict[j][1])
                    elif tmp < 3 and self.mistakes_corrected_oftenity[word] < int(self.input_dict[j][1]):
                        # In fact, this part of code is used to replace words with _most frequently_ words from
                        # dictionary.
                        self.mistakes_corrected[word] = dictionary_word
                        self.mistakes_corrected_oftenity[word] = int(self.input_dict[j][1])
                j += 1
        return

    def correctMistakes(self):  # Correcting mistakes at text
        i = 0
        for x in self.input_data:
            if x in self.mistakes_corrected:
                self.input_data[i] = self.mistakes_corrected[x]
            i += 1
        return

    def reset(self):  # Reset counters to repeat calculations after mistakes corrections
        self.unique_forms_in_dict_length = 0
        self.unique_forms = []
        self.unique_forms_length = 0
        return

    def get_result(self):  # Returns corrected data
        return self.input_data

    def run(self):  # Function used to simplify class usage from __main__
        self.findWordForms()
        self.findNeedle()
        self.findDistance()
        text.correctMistakes()
        self.reset()
        self.findWordForms()
        self.findNeedle()

    def printout(self):
        # print('Normalized input data:', self.input_data)  # debug
        # print('Normalized input dictionary:', self.input_dict)  # debug
        # print('Normalized operating dictionary:', self.normalized_dict)  # debug
        print('Word forms:', self.input_data_length)
        print('Unique word forms:', self.unique_forms_length)
        print('Unique word forms in input dictionary:', self.unique_forms_in_dict_length)
        print('Unique word forms not in input dictionary:',
              self.unique_forms_length - self.unique_forms_in_dict_length)
        print('Unique word forms not in dictionary (probably, mistakes):', self.potential_mistakes)
        print('Levinstein distances for potential mistakes:', self.mistakes_distance)
        print('Potential mistakes corrected:', self.mistakes_corrected)
        self.reset()
        return


if __name__ == '__main__':
    fin = open('sample_text.txt', 'r')
    fdict = open('sample_dictionary.txt', 'r')

    text = Text()

    for line in fin:
        tmp = line.replace('\n', ' ').lower()
        text.push(tmp, 'text')

    for line in fdict:
        tmp = line.rstrip().split()
        text.push(tmp, 'dict')
    fdict.close()
    text.push('', 'done')

    text.run()  # Run the mistakes correction
    text.printout()  # Output result (displays at console)

    fout = open('output_text.txt', 'w')  # The following code used to write result text to output_text.txt
    fin.seek(0)

    res_text = text.get_result()
    ignored = [
        '!',
        '.',
        '?',
        ',',
        '—',
        ';',
        ':',
        '«',
        '»',
        '(',
        ')',
        '…'
    ]
    j = 0
    for line in fin:
        line = line.replace('\n', '').split()
        for word in line:
            if word in ignored:
                continue
            if res_text[j] != word.lower():  # This conditions saves uppercase chars and punctuation marks
                if word[len(word) - 1] in ignored:
                    res_text[j] += (word[len(word) - 1])
                x = res_text[j]
            j += 1
            fout.write(word + ' ')
        fout.write('\n')
    fout.close()
    fin.close()
    exit(0)
