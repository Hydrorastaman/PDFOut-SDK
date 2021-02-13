#include <font/TrueType/Table.h>

#include <string>
#include <algorithm>

namespace kernel{ namespace font{ namespace truetype{

  TrueTypeTable getTableId(char tag[4]){
    std::string tagl(tag, tag + 4);
    std::transform(std::begin(tagl), std::end(tagl), std::begin(tagl), tolower);
    if (tagl == "acnt") return TrueTypeTableAcnt;
    else if (tagl == "ankr") return TrueTypeTableAnkr;
    else if (tagl == "avar") return TrueTypeTableAvar;
    else if (tagl == "bdat") return TrueTypeTableBdat;
    else if (tagl == "bhed") return TrueTypeTableBhed;
    else if (tagl == "bloc") return TrueTypeTableBloc;
    else if (tagl == "bsln") return TrueTypeTableBsln;
    else if (tagl == "cmap") return TrueTypeTableCmap;
    else if (tagl == "cvar") return TrueTypeTableCvar;
    else if (tagl == "cvt ") return TrueTypeTableCvt ;
    else if (tagl == "ebsc") return TrueTypeTableEBSC;
    else if (tagl == "fdsc") return TrueTypeTableFdsc;
    else if (tagl == "feat") return TrueTypeTableFeat;
    else if (tagl == "fmtx") return TrueTypeTableFmtx;
    else if (tagl == "fond") return TrueTypeTableFond;
    else if (tagl == "fpgm") return TrueTypeTableFpgm;
    else if (tagl == "fvar") return TrueTypeTableFvar;
    else if (tagl == "gasp") return TrueTypeTableGasp;
    else if (tagl == "glyf") return TrueTypeTableGlyf;
    else if (tagl == "gvar") return TrueTypeTableGvar;
    else if (tagl == "hdmx") return TrueTypeTableHdmx;
    else if (tagl == "head") return TrueTypeTableHead;
    else if (tagl == "hhea") return TrueTypeTableHhea;
    else if (tagl == "hmtx") return TrueTypeTableHmtx;
    else if (tagl == "just") return TrueTypeTableJust;
    else if (tagl == "kern") return TrueTypeTableKern;
    else if (tagl == "kerx") return TrueTypeTableKerx;
    else if (tagl == "lcar") return TrueTypeTableLcar;
    else if (tagl == "loca") return TrueTypeTableLoca;
    else if (tagl == "ltag") return TrueTypeTableLtag;
    else if (tagl == "maxp") return TrueTypeTableMaxp;
    else if (tagl == "meta") return TrueTypeTableMeta;
    else if (tagl == "mort") return TrueTypeTableMort;
    else if (tagl == "morx") return TrueTypeTableMorx;
    else if (tagl == "name") return TrueTypeTableName;
    else if (tagl == "opbd") return TrueTypeTableOpbd;
    else if (tagl == "os/2") return TrueTypeTableOS2;
    else if (tagl == "post") return TrueTypeTablePost;
    else if (tagl == "prep") return TrueTypeTablePrep;
    else if (tagl == "prop") return TrueTypeTableProp;
    else if (tagl == "sbix") return TrueTypeTableSbix;
    else if (tagl == "trak") return TrueTypeTableTrak;
    else if (tagl == "vhea") return TrueTypeTableVhea;
    else if (tagl == "vmtx") return TrueTypeTableVmtx;
    else if (tagl == "xref") return TrueTypeTableXref;
    else if (tagl == "zapf") return TrueTypeTableZapf;
    else if (tagl == "cff ") return TrueTypeTableCFF;
    else if (tagl == "vorg") return TrueTypeTableVORG;
    else if (tagl == "ebdt") return TrueTypeTableEBDT;
    else if (tagl == "eblc") return TrueTypeTableEBLC;
    else if (tagl == "base") return TrueTypeTableBASE;
    else if (tagl == "gdef") return TrueTypeTableGDEF;
    else if (tagl == "gpos") return TrueTypeTableGPOS;
    else if (tagl == "gsub") return TrueTypeTableGSUB;
    else if (tagl == "jstf") return TrueTypeTableJSTF;
    else if (tagl == "dsig") return TrueTypeTableDSIG;
    else if (tagl == "ltsh") return TrueTypeTableLTSH;
    else if (tagl == "pclt") return TrueTypeTablePCLT;
    else if (tagl == "vdmx") return TrueTypeTableVDMX;
    else if (tagl == "dttf") return TrueTypeTableDttf;
    else return TrueTypeTableInvalid;
  }

  void tableIdToTag(TrueTypeTable id, char *tag){
    switch (id){
      case TrueTypeTableAcnt: memcpy(tag, "acnt", sizeof(uint8_t) * 4); break;
      case TrueTypeTableAnkr: memcpy(tag, "ankr", sizeof(uint8_t) * 4); break;
      case TrueTypeTableAvar: memcpy(tag, "avar", sizeof(uint8_t) * 4); break;
      case TrueTypeTableBdat: memcpy(tag, "bdat", sizeof(uint8_t) * 4); break;
      case TrueTypeTableBhed: memcpy(tag, "bhed", sizeof(uint8_t) * 4); break;
      case TrueTypeTableBloc: memcpy(tag, "bloc", sizeof(uint8_t) * 4); break;
      case TrueTypeTableBsln: memcpy(tag, "bsln", sizeof(uint8_t) * 4); break;
      case TrueTypeTableCmap: memcpy(tag, "cmap", sizeof(uint8_t) * 4); break;
      case TrueTypeTableCvar: memcpy(tag, "cvar", sizeof(uint8_t) * 4); break;
      case TrueTypeTableCvt : memcpy(tag, "cvt ", sizeof(uint8_t) * 4); break;
      case TrueTypeTableEBSC: memcpy(tag, "ebsc", sizeof(uint8_t) * 4); break;
      case TrueTypeTableFdsc: memcpy(tag, "fdsc", sizeof(uint8_t) * 4); break;
      case TrueTypeTableFeat: memcpy(tag, "feat", sizeof(uint8_t) * 4); break;
      case TrueTypeTableFmtx: memcpy(tag, "fmtx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableFond: memcpy(tag, "fond", sizeof(uint8_t) * 4); break;
      case TrueTypeTableFpgm: memcpy(tag, "fpgm", sizeof(uint8_t) * 4); break;
      case TrueTypeTableFvar: memcpy(tag, "fvar", sizeof(uint8_t) * 4); break;
      case TrueTypeTableGasp: memcpy(tag, "gasp", sizeof(uint8_t) * 4); break;
      case TrueTypeTableGlyf: memcpy(tag, "glyf", sizeof(uint8_t) * 4); break;
      case TrueTypeTableGvar: memcpy(tag, "gvar", sizeof(uint8_t) * 4); break;
      case TrueTypeTableHdmx: memcpy(tag, "hdmx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableHead: memcpy(tag, "head", sizeof(uint8_t) * 4); break;
      case TrueTypeTableHhea: memcpy(tag, "hhea", sizeof(uint8_t) * 4); break;
      case TrueTypeTableHmtx: memcpy(tag, "hmtx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableJust: memcpy(tag, "just", sizeof(uint8_t) * 4); break;
      case TrueTypeTableKern: memcpy(tag, "kern", sizeof(uint8_t) * 4); break;
      case TrueTypeTableKerx: memcpy(tag, "kerx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableLcar: memcpy(tag, "lcar", sizeof(uint8_t) * 4); break;
      case TrueTypeTableLoca: memcpy(tag, "loca", sizeof(uint8_t) * 4); break;
      case TrueTypeTableLtag: memcpy(tag, "ltag", sizeof(uint8_t) * 4); break;
      case TrueTypeTableMaxp: memcpy(tag, "maxp", sizeof(uint8_t) * 4); break;
      case TrueTypeTableMeta: memcpy(tag, "meta", sizeof(uint8_t) * 4); break;
      case TrueTypeTableMort: memcpy(tag, "mort", sizeof(uint8_t) * 4); break;
      case TrueTypeTableMorx: memcpy(tag, "morx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableName: memcpy(tag, "name", sizeof(uint8_t) * 4); break;
      case TrueTypeTableOpbd: memcpy(tag, "opbd", sizeof(uint8_t) * 4); break;
      case TrueTypeTableOS2 : memcpy(tag, "OS/2", sizeof(uint8_t) * 4); break;
      case TrueTypeTablePost: memcpy(tag, "post", sizeof(uint8_t) * 4); break;
      case TrueTypeTablePrep: memcpy(tag, "prep", sizeof(uint8_t) * 4); break;
      case TrueTypeTableProp: memcpy(tag, "prop", sizeof(uint8_t) * 4); break;
      case TrueTypeTableSbix: memcpy(tag, "sbix", sizeof(uint8_t) * 4); break;
      case TrueTypeTableTrak: memcpy(tag, "trak", sizeof(uint8_t) * 4); break;
      case TrueTypeTableVhea: memcpy(tag, "vhea", sizeof(uint8_t) * 4); break;
      case TrueTypeTableVmtx: memcpy(tag, "vmtx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableXref: memcpy(tag, "xref", sizeof(uint8_t) * 4); break;
      case TrueTypeTableZapf: memcpy(tag, "zapf", sizeof(uint8_t) * 4); break;
      case TrueTypeTableCFF : memcpy(tag, "cff" , sizeof(uint8_t) * 4); break;
      case TrueTypeTableVORG: memcpy(tag, "vorg", sizeof(uint8_t) * 4); break;
      case TrueTypeTableEBDT: memcpy(tag, "ebdt", sizeof(uint8_t) * 4); break;
      case TrueTypeTableEBLC: memcpy(tag, "eblc", sizeof(uint8_t) * 4); break;
      case TrueTypeTableBASE: memcpy(tag, "base", sizeof(uint8_t) * 4); break;
      case TrueTypeTableGDEF: memcpy(tag, "gdef", sizeof(uint8_t) * 4); break;
      case TrueTypeTableGPOS: memcpy(tag, "gpos", sizeof(uint8_t) * 4); break;
      case TrueTypeTableGSUB: memcpy(tag, "gsub", sizeof(uint8_t) * 4); break;
      case TrueTypeTableJSTF: memcpy(tag, "jstf", sizeof(uint8_t) * 4); break;
      case TrueTypeTableDSIG: memcpy(tag, "dsig", sizeof(uint8_t) * 4); break;
      case TrueTypeTableLTSH: memcpy(tag, "ltsh", sizeof(uint8_t) * 4); break;
      case TrueTypeTablePCLT: memcpy(tag, "pclt", sizeof(uint8_t) * 4); break;
      case TrueTypeTableVDMX: memcpy(tag, "vdmx", sizeof(uint8_t) * 4); break;
      case TrueTypeTableDttf: memcpy(tag, "dttf", sizeof(uint8_t) * 4); break;
    }
  }

  Table::Table(void){
  }

  Table::~Table(void){
  }

}}}
