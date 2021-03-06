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



// Compress in-data with huffman compression algorithm, and save the frequency table in the output header.
void compress(istream& input, obitstream& output)
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


// Decompresses our huffman compressed indata with the help of the information in the header.
void decompress(ibitstream& input, ostream& output)
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
