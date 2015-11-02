#include "tree_sitter/compiler.h"
#include "helpers.h"

namespace tree_sitter_examples {

// http://slps.github.io/zoo/c/iso-9899-tc3.html

extern const Grammar c = Grammar({
  { "program", repeat(choice({
    sym("preproc_define"),
    sym("function_definition"),
    sym("declaration") })) },

  { "preproc_define", seq({
    str("#define"),
    sym("identifier"),
    optional(token(prec(-1, repeat1(choice({ str("\\\n"), pattern(".") }))))),
    str("\n") }) },

  { "function_definition", seq({
    optional(sym("declaration_specifiers")),
    sym("_type_specifier"),
    sym("declarator"),
    repeat(sym("declaration")),
    sym("compound_statement") }) },

  { "declaration_specifiers", repeat1(choice({
    sym("storage_class_specifier"),
    sym("type_qualifier") })) },

  { "storage_class_specifier", choice({
    str("typedef"),
    str("extern"),
    str("static"),
    str("auto"),
    str("register") }) },

  { "_type_specifier", choice({
    sym("struct_specifier"),
    sym("type_name") }) },

  { "type_name", seq({
    repeat(choice({
      str("signed"),
      str("unsigned"),
      str("long"),
      str("short") })),
    sym("identifier") }) },

  { "struct_specifier", seq({
    str("struct"),
    optional(sym("identifier")),
    seq({
      str("{"),
      repeat(sym("struct_declaration")),
      str("}") }) }) },

  { "struct_declaration", seq({
    sym("_type_specifier"),
    sym("declarator") }) },

  { "parameter_declaration", seq({
    optional(sym("declaration_specifiers")),
    sym("_type_specifier"),
    sym("declarator") }) },

  { "declaration", seq({
    optional(sym("declaration_specifiers")),
    sym("_type_specifier"),
    comma_sep1(sym("_init_declarator")),
    str(";") }) },

  { "_init_declarator", choice({
    sym("declarator"),
    seq({ sym("declarator"), str("="), sym("initializer") }) }) },

  { "initializer", choice({
    sym("expression"),
    seq({
      str("{"),
      sym("initializer_list"),
      optional(str(",")),
      str("}") }) }) },

  { "initializer_list", choice({
    seq({
      optional(sym("designation")),
      sym("initializer") }),
    seq({
      sym("initializer_list"),
      str(","),
      optional(sym("designation")),
      sym("initializer") }) }) },

  { "designation", seq({
    repeat1(choice({
      seq({
        str("["),
        sym("expression"),
        str("]") }),
      seq({
        str("."),
        sym("identifier") }) })),
    str("=") }) },

  { "declarator", seq({
    repeat(sym("pointer")),
    sym("_direct_declarator") }) },

  { "_direct_declarator", choice({
    sym("identifier"),
    seq({
      str("("),
      sym("declarator"),
      str(")") }),
    seq({
      sym("_direct_declarator"),
      str("["),
      optional(sym("expression")),
      str("]") }),
    seq({
      sym("_direct_declarator"),
      str("("),
      comma_sep(sym("parameter_declaration")),
      str(")") }) }) },

  { "type_qualifier", choice({
    str("const"),
    str("restrict"),
    str("volatile") }) },

  { "pointer", str("*") },

  { "compound_statement", seq({
    str("{"),
    err(repeat(choice({ sym("declaration"), sym("statement") }))),
    str("}") }) },

  { "expression", choice({
    sym("math_expression"),
    sym("call_expression"),
    sym("string"),
    sym("identifier"),
    sym("number") }) },

  { "math_expression", choice({
    prec_left(1, seq({ sym("expression"), str("+"), sym("expression") })),
    prec_left(2, seq({ sym("expression"), str("*"), sym("expression") })) }) },

  { "call_expression", prec(3, seq({
    sym("expression"),
    str("("),
    comma_sep(sym("expression")),
    str(")") })) },

  { "statement", choice({
    sym("expression_statement") }) },

  { "expression_statement", seq({
    sym("expression"),
    str(";") }) },

  { "string", delimited("\"") },

  { "identifier", pattern("\\a[\\w_]*") },

  { "number", pattern("\\d+(\\.\\d+)?") },

  { "comment", pattern("//[^\n]*") },
}).ubiquitous_tokens({
  sym("comment"),
  pattern("[ \t\r\n]"),
}).expected_conflicts({
  { "type_name", "expression" },
});

}  // namespace tree_sitter_examples
