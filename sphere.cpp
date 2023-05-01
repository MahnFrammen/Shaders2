
Decl: PtrDecl
    | StmtList
    | VarDecl
;

PtrDeclList:
	| PtrDecl PtrDeclList {$1->left = $2;
				$$ = $1;}
	|PtrDecl{$$ = $1};

PtrDecl: INT ID NUM SEMICOLON INT ASTERICK PTR EQ AMBERSAND ID SEMICOLON 
		ASTERICK PTR EQ NUM SEMICOLON {
		printf("\nRECOGNIZED RULE: Pointer"); 
		char id1[50];
		char type[50];
		strcpy(id1, $2);
		strcpy(type, $1);
		strcpy(scope, Local);
		int inSymTab = found($2, scope);
								
		if (inSymTab == 0){
		add_Item_to_Ptr_sym_table(id1, "Ptr", $1, 0, scope);

		add_Item_to_Ptr_sym_table(id1, "Ptr", &type, 0, &id1, &scope);
		}
		else{					
		printf("SEMANTIC ERROR: Ptr %s is already in the symbol table", $2);
								}
		showVarSymTable();
		showPtrSymTable();

		$$ = AST_Type("PTR", $1, $2);
			

if (int x -> ptr1) {
write ptr1;
printf("This is semantically correct")
}
else { 
printf("SEMANTIC ERROR: The values are not sequential and the value of ptr %s is invalid"
}


#### IR Code ####

T0 = x 
T0 = 12
T1 = ptr1
T1 = 24

 
#### Assembly Code ####

.main:

# ---------
li $t0, 12 //int x
li $t1, 24 //ptr value 

mov $t0, eax // $t0 is now eax
mov $t1, ebx // $t1 is now going to the value of ebx
mov [eax], ebx //register $t01 is now pointed to $t1 value

# -------------

li $v0, 10
syscall
.end main


#### Optimized IR Code  ####

T0 = x
T0 = eax 
T0 = 12
T1 = ptr1
T1 = 24
T1 = ebx

#### Optimized Assmebly Code ####

.main:

# ---------
li eax, 12 //int x
li ebx, 24 //ptr value 

mov [eax], ebx //register $t01 is now pointed to $t1 value

# -------------

li $v0, 10
syscall
.end main


--- Abstract Syntax Tree ---

Type 
int x
    int 12
         type 
         char ptr 
               int 24 
                    write ptr1






















