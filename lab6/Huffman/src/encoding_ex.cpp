// anden259 andno037

#include "encoding.h"
#include <queue>

// builds a table with the count of each byte in the input data
map<int, int> buildFrequencyTable(istream& input)
{
    map<int, int> freqTable;

    while (true) {
        int byte = 0;
        byte = input.get();
        if (byte == -1) {
            ++freqTable[PSEUDO_EOF];
            break;
        } else {
            ++freqTable[byte];
        }

    }
    return freqTable;
}

// builds a huffman tree from the frequency table
HuffmanNode* buildEncodingTree(const map<int, int> &freqTable)
{

    priority_queue<HuffmanNode> my_queue;
    for (auto pair : freqTable) {
        my_queue.push(HuffmanNode {pair.first, pair.second});
    }

    while (my_queue.size() != 1) {
        HuffmanNode loc_root;
        HuffmanNode* left = new HuffmanNode {my_queue.top()};
        my_queue.pop();
        HuffmanNode* right = new HuffmanNode {my_queue.top()};
        my_queue.pop();

        loc_root.count = left->count + right->count;
        loc_root.zero = left;
        loc_root.one = right;

        my_queue.push(loc_root);
    }
    return new HuffmanNode {my_queue.top()};
}

// Recursive help funktion that builds the encoding map from the huffman tree.
void buildEncodingMapRecursive(HuffmanNode * node, map<int , string> &encodingMap, string bin = "")
{
    if (node->character != NOT_A_CHAR) {
        encodingMap[node->character] = bin;
    } else {
        buildEncodingMapRecursive(node->zero, encodingMap, bin + "0");
        buildEncodingMapRecursive(node->one, encodingMap, bin + "1");
    }
}

// Builds the encoding map from the huffman tree
map<int, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    map<int, string> encodingMap;
    buildEncodingMapRecursive(encodingTree, encodingMap);
    return encodingMap;
}

// Encodes the in-data
void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output)
{
    bool run = true;
    while (run) {
        int byte = 0;
        byte = input.get();
        string code;
        if (byte == -1) { // -1 is EOF
            code = encodingMap.at(PSEUDO_EOF);
            run = false;
        } else {
            code = encodingMap.at(byte);
        }
        for (auto c : code) {
            if (c == '0') {
                output.writeBit(0);
            } else if (c == '1') {
                output.writeBit(1);
            } else {
                cerr << "error\n";
            }
        }
    }
}

// Decodes the in-data with an encoding tree.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output)
{
    HuffmanNode* current = encodingTree;
    while (true) {
        if (current->character == PSEUDO_EOF) {
            break;
        } else if (current->character != NOT_A_CHAR) {
            output << (char)current->character;
            current = encodingTree;
        } else {
            bool bit = input.readBit();
            if (bit) {
                current = current->one;
            } else {
                current = current->zero;
            }
        }
    }
}

/* Header structure
 *
 * +-----------------+-----------------+-----------------+ -  -  - +-----------------+-----------------+ -  -  -
 * | #N header-pairs | byte            | frequency       |  .....  | N'th       byte | N'th frequency  | compressed data after header
 * | 1-byte          | 1-byte          | 1-4 bytes       |         |                 |                 |
 * +-----------------+-----------------+-----------------+ -  -  - +-----------------+-----------------+ -  -  -
 *
 * how frequency works
 * the leading non-zero bits tells us how many extra bytes we need to hold the frequency of a byte
 * e.g.
 *
 * 00000001 is 1
 * 01111111 is 127, and the most that we can fit in 1 byte.
 *
 * 10000000 10000000 is 128 and requires 2 bytes to be represented.
 * 10111111 11111111 is 16383 and the most that fit in 2 bytes
 *
 * so the leading 1-set bits tells us how many extra bytes we need to represent the frequency
 * since the given code only support int's we decided to support a maximum of four extra bytes, that is almost one int.
 *
 */

// Help function to write the header while encoding.
void help_write_header_count(obitstream& output, unsigned int number)
{
    if (number <= 0x7f) {
        output << (char) number;
    } else if (number <= 0x3fff) {
        output << (char)((number >> 8) | 0x80) << (char) number;
    } else if (number <= 0x1fffff) {
        output << (char)((number >> 16) | 0xC0) << (char)(number >> 8) << (char) number;
    } else if (number <= 0xfffffff) {
        output << (char)((number >> 24) | 0xE0) << (char)(number >> 16) << (char)(number >> 8) << (char) number;
    }
}

// Compress in-data with huffman compression algorithm, and save the frequency table in the output header.
void compress(istream& input, obitstream& output)
{
    map<int, int>freqTable = buildFrequencyTable(input);
    // ignore eof
    output << (char)(freqTable.size() - 1);

    for (auto pair : freqTable) {
        if (pair.first != PSEUDO_EOF) {
            output << (char)pair.first;
            help_write_header_count(output, pair.second);
        }
    }

    input.clear();
    input.seekg(0, input.beg);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    encodeData(input, buildEncodingMap(tree), output);
    freeTree(tree);
}



// Help function for reading the header while decoding.
int help_read_header_count(istream& input)
{
    unsigned int byte1 = input.get();
    if((byte1 & 0xe0) == 0xe0) {
        unsigned int byte2 = input.get();
        unsigned int byte3 = input.get();
        unsigned int byte4 = input.get();
        return ((byte1 & 0x0f) << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    } else if((byte1 & 0xc0) == 0xc0) {
        unsigned int byte2 = input.get();
        unsigned int byte3 = input.get();
        return ((byte1 & 0x1f) << 16) | (byte2 << 8) | byte3;
    } else if((byte1 & 0x80) == 0x80) {
        unsigned int byte2 = input.get();
        return ((byte1 & 0x3f) << 8) | byte2;
    } else {
        return byte1;
    }
}

// Decompresses our huffman compressed indata with the help of the information in the header.
void decompress(ibitstream& input, ostream& output)
{
    map<int, int>freqTable;

    int header_pairs = input.get();
    for (int i = 0; i < header_pairs; ++i) {
        int character = input.get();
        int counter = help_read_header_count(input);
        freqTable[character] = counter;
    }
    freqTable[PSEUDO_EOF] = 1;

    HuffmanNode* tree = buildEncodingTree(freqTable);

    decodeData(input, tree, output);
    freeTree(tree);
}


// dealloc a huffman tree.
void freeTree(HuffmanNode* node)
{
    if (node == nullptr)
        return;
    if (node->zero != nullptr)
        freeTree(node->zero);
    if (node->one != nullptr)
        freeTree(node->one);
    delete node;
}
