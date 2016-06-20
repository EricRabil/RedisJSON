#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "minunit.h"
#include "../src/path.h"
#include "../src/object.h"

MU_TEST(testObject) {

    Node *root = NewObjectNode(1);
    mu_check (root != NULL);
    
    mu_check (OBJ_OK == Node_ObjSet(root, "foo", NewStringNode("bar", 3)));
    mu_check (OBJ_OK == Node_ObjSet(root, "bar", NewBoolNode(0)));
    mu_check (OBJ_OK == Node_ObjSet(root, "baz", NewArrayNode(0)));

    Node *arr, *n;
    int rc =  Node_ObjGet(root, "non existing", &arr);
    mu_assert_int_eq(OBJ_ERR, rc);

    rc =  Node_ObjGet(root, "baz", &arr);
    mu_assert_int_eq(OBJ_OK, rc);
    
    mu_check (arr != NULL);

    mu_assert_int_eq(Node_ArrayAppend(arr, NewDoubleNode(3.141)), OBJ_OK);
    mu_assert_int_eq(Node_ArrayAppend(arr, NewIntNode(1337)), OBJ_OK);
    mu_assert_int_eq(Node_ArrayAppend(arr, NewStringNode("foo", 3)), OBJ_OK);
    mu_assert_int_eq(Node_ArrayAppend(arr, NULL), OBJ_OK);

    rc = Node_ArrayItem(arr, 0, &n);
    mu_assert_int_eq(OBJ_OK, rc);
    
    mu_check (n != NULL);
    mu_check (n->type == N_NUMBER);


   Node_Print(root, 0);
   Node_Free(root);
}

MU_TEST(testPath) {

    Node *root = NewObjectNode(1);
    mu_check (root != NULL);
    
    mu_check (OBJ_OK == Node_ObjSet(root, "foo", NewStringNode("bar", 3)));
    mu_check (OBJ_OK == Node_ObjSet(root, "bar", NewBoolNode(0)));

    Node *arr = NewArrayNode(0);
    Node_ArrayAppend(arr, NewStringNode("hello", 5));
    Node_ArrayAppend(arr, NewStringNode("world", 5));

    mu_check (OBJ_OK == Node_ObjSet(root, "baz", arr));

    SearchPath sp = NewSearchPath(2);
    SearchPath_AppendKey(&sp, "baz");
    SearchPath_AppendIndex(&sp, 0);

    Node *n = NULL;
    PathError pe = SearchPath_Find(&sp, root, &n);
    
    mu_check (pe == E_OK);
    mu_check ( n != NULL);

    mu_check (n->type == N_STRING);
    mu_check (!strcmp(n->value.strval.data, "hello"));

    Node_Print(n,0);

}

MU_TEST_SUITE(test_object) {
	//MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(testObject);
    MU_RUN_TEST(testPath);
	
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_object);
	MU_REPORT();
	return 0;
}
