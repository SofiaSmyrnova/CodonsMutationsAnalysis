#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

set<string> endcodons = {
    "TAA", "TAG", "TGA"
};

bool itisnucleotide(unsigned char c) {
    c = toupper(c);
    return (c == 'A' || c == 'T' || c == 'G' || c == 'C');
    }

vector<string> extractingcodons(const string& line) {
    vector<string> codons;
    string cleaned = "";

    for (unsigned char c : line) {
        if (itisnucleotide(c)) {
            cleaned += toupper(c);
        }
    }

    for (int i = 0; i + 3 <= cleaned.size(); i += 3 ) {
        codons.push_back(cleaned.substr(i, 3 ));
    }
  

    if (cleaned.size() % 3 != 0) {
        cout << "Mutation was found in codons count!\n";
    }
    return codons;
}

bool itisvalidcodon(const string& codon) {
    return codon.length() == 3 && !endcodons.count(codon);
}

void results(
    const vector<string>& result,
    const map<string, int>& frequency,
    ostream& out)
{
    out << " Palindrome codons found:\n";

    if (result.empty()) {
        out << " No palindrome codons found.\n";
    }
    else {
        for (const string& word : result) {
            out << "  " << word;
                out << " - nucleotides: " << word.length();
        }
    }

    out << " Total palindrome codons: ";
    out << " Codon frequency in sequence:\n";

    vector<pair<string, int>> freqvec(frequency.begin(), frequency.end());
    sort(freqvec.begin(), freqvec.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });

    for (const auto& p : freqvec) {
        out << "  " << p.first << " -> " << p.second; out << " time(s) - ";
    }
}

int main() {
    vector<string> result;
    map<string, int> frequency;

    map<string, string> descriptions;
    ifstream fdesc("descriptions.txt");
    string descLine;
    while (getline(fdesc, descLine)) {
        int pos = descLine.find(':');
        string codon = descLine.substr(0, pos);
        string desc = descLine.substr(pos + 1);
        descriptions[codon] = desc;
    }

    set<string> knownmutations;
    ifstream fmut("mutations.txt");
    string m;
    while (getline(fmut, m)) {
        knownmutations.insert(m);
    }

    ifstream fin("input.txt");
    string line;
    while (getline(fin, line)) {
        vector<string> codons = extractingcodons(line);

        if (codons[0] == "ATG") {
            cout << "Valid start codon ATG found\n";
        }
        else {
            cout << "Mutation! Expected ATG but found: " << codons[0] << "\n";
        }
        for (const string& codon : codons) {
            frequency[codon]++;
            if (!itisvalidcodon(codon)) {
                cout << "Invalid/stop codon found: " << codon << "\n";
            }
            if (knownmutations.count(codon)) {
                cout << "Known mutation found: " << codon << "\n";
                cout << "  -> " << descriptions[codon] << "\n";
            }
            if (codon.front() == codon.back()) {
                result.push_back(codon);
            }
        }
    }

    results(result, frequency, cout);
}