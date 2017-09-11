#include "pattern_serializer.h"

MP7PatternSerializer::MP7PatternSerializer(const std::string &fname, const std::string &boardName) :
    fname_(fname), file_(nullptr), ipattern_(0) 
{
    if (!fname.empty()) {
        file_ = fopen(fname.c_str(), "w");
        fprintf(file_, "Board %s\n", boardName.c_str());
        fprintf(file_, " Quad/Chan :    q00c0      q00c1      q00c2      q00c3      q01c0      q01c1      q01c2      q01c3      q02c0      q02c1      q02c2      q02c3      q03c0      q03c1      q03c2      q03c3      q04c0      q04c1      q04c2      q04c3      q05c0      q05c1      q05c2      q05c3      q06c0      q06c1      q06c2      q06c3      q07c0      q07c1      q07c2      q07c3      q08c0      q08c1      q08c2      q08c3      q09c0      q09c1      q09c2      q09c3      q10c0      q10c1      q10c2      q10c3      q11c0      q11c1      q11c2      q11c3      q12c0      q12c1      q12c2      q12c3      q13c0      q13c1      q13c2      q13c3      q14c0      q14c1      q14c2      q14c3      q15c0      q15c1      q15c2      q15c3      q16c0      q16c1      q16c2      q16c3      q17c0      q17c1      q17c2      q17c3  \n");
        fprintf(file_, "      Link :     00         01         02         03         04         05         06         07         08         09         10         11         12         13         14         15         16         17         18         19         20         21         22         23         24         25         26         27         28         29         30         31         32         33         34         35         36         37         38         39         40         41         42         43         44         45         46         47         48         49         50         51         52         53         54         55         56         57         58         59         60         61         62         63         64         65         66         67         68         69         70         71    \n");
    }
}

MP7PatternSerializer::~MP7PatternSerializer() 
{
    if (file_) {
        fclose(file_); file_ = nullptr;
        printf("Saved %u MP7 patterns to %s.\n", ipattern_, fname_.c_str());
    }
}

void MP7PatternSerializer::operator()(const MP7DataWord event[MP7_NCHANN]) 
{
    if (!file_) return;
    fprintf(file_, "Frame %04u :", ipattern_);
    for (unsigned int i = 0; i < MP7_NCHANN; ++i) {
        fprintf(file_, " 1v%08x", unsigned(event[i]));
    }
    fprintf(file_, "\n");
    ipattern_++;
}

HumanReadablePatternSerializer::HumanReadablePatternSerializer(const std::string &fname) :
    fname_(fname), file_(nullptr), ipattern_(0) 
{
    if (!fname.empty()) {
        if (fname == "-") {
            file_ = stdout;
        } else {
            file_ = fopen(fname.c_str(), "w");
        }
    }
}

HumanReadablePatternSerializer::~HumanReadablePatternSerializer() 
{
    if (file_ && (file_ != stdout)) {
        fclose(file_); 
        printf("Saved %u human readable patterns to %s.\n", ipattern_, fname_.c_str());
    }
}

void HumanReadablePatternSerializer::operator()(const EmCaloObj emcalo[NEMCALO], const HadCaloObj hadcalo[NCALO], const TkObj track[NTRACK], const PFChargedObj outch[NTRACK], const PFNeutralObj outpho[NPHOTON], const PFNeutralObj outne[NSELCALO]) 
{
    if (!file_) return;
    fprintf(file_, "Frame %04u:\n", ipattern_);
    dump_inputs(emcalo,hadcalo,track);
    dump_outputs(outch,outpho,outne);
    fprintf(file_, "\n");
    if (file_ == stdout) fflush(file_);
    ipattern_++;
}

void HumanReadablePatternSerializer::dump_inputs(const EmCaloObj emcalo[NEMCALO], const HadCaloObj hadcalo[NCALO], const TkObj track[NTRACK]) 
{
    for (int i = 0; i < NCALO; ++i) {
        fprintf(file_, "   calo  %3d, hwPt % 7d   hwEmPt  % 7d    hwEta %+7d   hwPhi %+7d   hwIsEM %1d\n", i, int(hadcalo[i].hwPt), int(hadcalo[i].hwEmPt), int(hadcalo[i].hwEta), int(hadcalo[i].hwPhi), int(hadcalo[i].hwIsEM));
    }
    for (int i = 0; i < NEMCALO; ++i) {
        fprintf(file_, "   em    %3d, hwPt % 7d   hwPtErr % 7d    hwEta %+7d   hwPhi %+7d\n", i, int(emcalo[i].hwPt), int(emcalo[i].hwPtErr), int(emcalo[i].hwEta), int(emcalo[i].hwPhi));
    }
    for (int i = 0; i < NTRACK; ++i) {
        fprintf(file_, "   track %3d, hwPt % 7d   hwPtErr % 7d    hwEta %+7d   hwPhi %+7d     hwZ0 %+7d\n", i, int(track[i].hwPt), int(track[i].hwPtErr), int(track[i].hwEta), int(track[i].hwPhi), int(track[i].hwZ0));
    }
    if (file_ == stdout) fflush(file_);
}

void HumanReadablePatternSerializer::dump_outputs(const PFChargedObj outch[NTRACK], const PFNeutralObj outpho[NPHOTON], const PFNeutralObj outne[NSELCALO]) 
{
    for (int i = 0; i < NTRACK; ++i) {
        fprintf(file_, "   charged pf %3d, hwPt % 7d   hwEta %+7d   hwPhi %+7d   hwId %1d      hwZ0 %+7d\n", i,
                int(outch[i].hwPt), int(outch[i].hwEta), int(outch[i].hwPhi), int(outch[i].hwId), int(outch[i].hwZ0));
    }
    for (int i = 0; i < NPHOTON; ++i) {
        fprintf(file_, "   photon  pf %3d, hwPt % 7d   hwEta %+7d   hwPhi %+7d   hwId %1d\n", i,
                int(outpho[i].hwPt), int(outpho[i].hwEta), int(outpho[i].hwPhi), int(outpho[i].hwId));
    }
    for (int i = 0; i < NSELCALO; ++i) {
        fprintf(file_, "   neutral pf %3d, hwPt % 7d   hwEta %+7d   hwPhi %+7d   hwId %1d\n", i,
                int(outne[i].hwPt), int(outne[i].hwEta), int(outne[i].hwPhi), int(outne[i].hwId));
    }
    if (file_ == stdout) fflush(file_);
}
  
