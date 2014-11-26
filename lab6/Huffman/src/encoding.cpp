// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
// TODO: include any other headers you need

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

void buildEncodingMapRecursive(HuffmanNode * node, map<int , string> &encodingMap, string bin = "")
{
    if (node->character != NOT_A_CHAR) {
        encodingMap[node->character] = bin;
    } else {
        buildEncodingMapRecursive(node->zero, encodingMap, bin + "0");
        buildEncodingMapRecursive(node->one, encodingMap, bin + "1");
    }
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    map<int, string> encodingMap;
    buildEncodingMapRecursive(encodingTree, encodingMap);
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output)
{
    bool run = true;
    while (run) {
        int byte = 0;
        byte = input.get();
        string code;
        if (byte == -1) {
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
void help_write_header_count(obitstream& output, unsigned int number)
{
    if (number <= 0x7f) {
        output << (char) number;
    } else if (number <= 0x3fff) {
        /*
        short tmp = tal | 0x8000;
        char msb = tmp >> 8, lsb = tmp;
        output << msb << lsb;
        //*/
        //output << (((char) (number >> 8)) | 0x80) << (char) number;
        output << (char)((number >> 8) | 0x80) << (char) number;
    } else if (number <= 0x1fffff) {
        /*
        int tmp=tal | 0xc00000;
        char left = tmp >> 16, mid = tmp >> 8, right;
        output << left << mid << right;
        //*/
        output << (char)((number >> 16) | 0xC0) << (char)(number >> 8) << (char) number;
    } else if (number <= 0xfffffff) {
        output << (char)((number >> 24) | 0xE0) << (char)(number >> 16) << (char)(number >> 8) << (char) number;
    }

}

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

void compress_old(istream& input, obitstream& output)
{
    map<int, int>freqTable = buildFrequencyTable(input);
    output << "{";
    bool first = true;
    for (auto pair : freqTable) {
        if (!first) {
            output << ", " << pair.first << ":" << pair.second;
        } else {
            output << pair.first << ":" << pair.second;
            first = false;
        }
    }
    output << "}";

    input.clear();
    input.seekg(0, input.beg);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    encodeData(input, buildEncodingMap(tree), output);
    freeTree(tree);
}

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

void decompress_old(ibitstream& input, ostream& output)
{
    map<int, int>freqTable;
    int character = 0;
    int count = 0;
    string ascii;

    input.get();
    bool run = true;
    while (run) {
        character = 0;
        count = 0;
        while (true) {
            char c = input.get();
            if (c == ':') {
                character = stoi(ascii);
                ascii.clear();
                break;
            } else {
                ascii += c;
            }
        }
        while (true) {
            char c = input.get();
            if (c == ',') {
                input.get();
                count = stoi(ascii);
                ascii.clear();
                break;
            } else if (c == '}') {
                count = stoi(ascii);
                ascii.clear();
                run = false;
                break;
            } else {
                ascii += c;
            }
        }
        freqTable[character] = count;
    }

    HuffmanNode* tree = buildEncodingTree(freqTable);

    decodeData(input, tree, output);
    freeTree(tree);
}

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
