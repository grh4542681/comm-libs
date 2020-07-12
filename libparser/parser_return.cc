#include "parser_return.h"

namespace parser {

ParserRet::ECodeMapType ParserRet::ECodeMap = {
    { ECode::PARSER_EBASE, "" },
    { ECode::PARSER_ENOTFOUND, "" },
    { ECode::PARSER_ELOCK, "" },
    { ECode::PARSER_EGETRLOCK, "" },
    { ECode::PARSER_EGETWLOCK, "" },
    { ECode::PARSER_EUNLOCK, "" },
    { ECode::PARSER_ETYPE, "" },
};

}
