#ifndef __structs_t
#define __structs_t
// Ast Structs
typedef struct Program_s            Program;
typedef struct ClassDecl_s          ClassDecl;
typedef struct FunctionDecl_s       FunctionDecl;
typedef struct ClassMember_s        ClassMember;
typedef struct ConstrDecl_s         ConstrDecl;
typedef struct Assignment_s         Assignment;
typedef struct Var_s                Var;
typedef struct Suite_s              Suite;
typedef struct StmtList_s           StmtList;
typedef struct ArgList_s            ArgList;
typedef struct FormalArgs_s         FormalArgs;
typedef struct VarDecl_s            VarDecl;
typedef struct Exp_s                Exp;
typedef struct BinOp_s              BinOp;
typedef struct Object_s             Object;
typedef struct Return_s             Return;
typedef struct FieldAccess_s        FieldAccess;
typedef struct MethodInvoc_s        MethodInvoc;
typedef struct IdList_s             IdList;
typedef struct New_s                New;
typedef struct Int_s                Int;
typedef struct Bool_s               Bool;
typedef struct IfStmt_s             IfStmt;
typedef struct WhileStmt_s          WhileStmt;
typedef struct ReturnStmt_s         ReturnStmt;
typedef struct Primary_s         Primary;

// Symbol Table Structs
typedef struct ClassTable_s ClassTable;
typedef struct Class_s Class;
typedef struct Function_s Function; 
typedef struct Variable_s Variable;
typedef struct Type_s Type;
#endif

