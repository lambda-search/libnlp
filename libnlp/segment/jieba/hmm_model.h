

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include "libnlp/base/string_util.h"
#include "libnlp/segment/jieba/trie.h"
#include <unordered_set>

namespace libnlp::jieba {

    using namespace libnlp;
    typedef std::unordered_map<rune_t, double> emit_prob_map;

    struct hmm_model {
        /*
         * STATUS:
         * 0: hmm_model::B, 1: hmm_model::E, 2: hmm_model::M, 3:hmm_model::S
         * */
        enum {
            B = 0, E = 1, M = 2, S = 3, STATUS_SUM = 4
        };

        hmm_model(const std::string &modelPath) {
            memset(startProb, 0, sizeof(startProb));
            memset(transProb, 0, sizeof(transProb));
            statMap[0] = 'B';
            statMap[1] = 'E';
            statMap[2] = 'M';
            statMap[3] = 'S';
            emitProbVec.push_back(&emitProbB);
            emitProbVec.push_back(&emitProbE);
            emitProbVec.push_back(&emitProbM);
            emitProbVec.push_back(&emitProbS);
            load_model(modelPath);
        }

        ~hmm_model() {
        }

        void load_model(const std::string &filePath) {
            std::ifstream ifile(filePath.c_str());
            XCHECK(ifile.is_open()) << "open " << filePath << " failed";
            string line;
            vector<string> tmp;
            vector<string> tmp2;
            //Load startProb
            XCHECK(get_line(ifile, line));
            Split(line, tmp, " ");
            XCHECK(tmp.size() == STATUS_SUM);
            for (size_t j = 0; j < tmp.size(); j++) {
                startProb[j] = atof(tmp[j].c_str());
            }

            //Load transProb
            for (size_t i = 0; i < STATUS_SUM; i++) {
                XCHECK(get_line(ifile, line));
                Split(line, tmp, " ");
                XCHECK(tmp.size() == STATUS_SUM);
                for (size_t j = 0; j < STATUS_SUM; j++) {
                    transProb[i][j] = atof(tmp[j].c_str());
                }
            }

            //Load emitProbB
            XCHECK(get_line(ifile, line));
            XCHECK(load_emit_prob(line, emitProbB));

            //Load emitProbE
            XCHECK(get_line(ifile, line));
            XCHECK(load_emit_prob(line, emitProbE));

            //Load emitProbM
            XCHECK(get_line(ifile, line));
            XCHECK(load_emit_prob(line, emitProbM));

            //Load emitProbS
            XCHECK(get_line(ifile, line));
            XCHECK(load_emit_prob(line, emitProbS));
        }

        double get_emit_prob(const emit_prob_map *ptMp, rune_t key,
                           double defVal) const {
            emit_prob_map::const_iterator cit = ptMp->find(key);
            if (cit == ptMp->end()) {
                return defVal;
            }
            return cit->second;
        }

        bool get_line(std::ifstream &ifile, string &line) {
            while (getline(ifile, line)) {
                auto sv = libnlp::trim_all(line, " ");
                if (sv.empty()) {
                    continue;
                }
                if (sv[0] == '#') {
                    continue;
                }
                return true;
            }
            return false;
        }

        bool load_emit_prob(const std::string &line, emit_prob_map &mp) {
            if (line.empty()) {
                return false;
            }
            vector<string> tmp, tmp2;
            unicode unicode;
            Split(line, tmp, ",");
            for (size_t i = 0; i < tmp.size(); i++) {
                Split(tmp[i], tmp2, ":");
                if (2 != tmp2.size()) {
                    XLOG(ERROR) << "emitProb illegal.";
                    return false;
                }
                if (!decode_runes_in_string(tmp2[0], unicode) || unicode.size() != 1) {
                    XLOG(ERROR) << "TransCode failed.";
                    return false;
                }
                mp[unicode[0]] = atof(tmp2[1].c_str());
            }
            return true;
        }

        char statMap[STATUS_SUM];
        double startProb[STATUS_SUM];
        double transProb[STATUS_SUM][STATUS_SUM];
        emit_prob_map emitProbB;
        emit_prob_map emitProbE;
        emit_prob_map emitProbM;
        emit_prob_map emitProbS;
        vector<emit_prob_map *> emitProbVec;
    }; // struct hmm_model

} // namespace libnlp::jieba
