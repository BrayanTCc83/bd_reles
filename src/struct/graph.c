#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structs.h"
#include "../utils/utils.h"

extern const char *INSUFICIENT_MEMORY, *GRAPH_TYPE, *NODE_TYPE, *NO_ERROR;
extern data_type_t DB_INT;

graph_t *new_graph(types_t type) {
	graph_t *Graph = (graph_t*) malloc(sizeof(graph_t));
	if(Graph == NULL)
		PrintError(INSUFICIENT_MEMORY, GRAPH_TYPE);

	Graph->o = GRAPH;
	Graph->nodes = new_linked_list(N_NODE);
	Graph->type = type;
	Graph->size = 0;
	return Graph;
}

graph_t *clone_graph(const graph_t graph) {
	graph_t *Graph = new_graph(graph.type);
	Graph->nodes = clone_linked_list(*graph.nodes);
	Graph->size = graph.size;
	return Graph;
}

compare_result_t compare_graph(graph_t graph1, graph_t graph2) {
	return compare_linked_list(*graph1.nodes, *graph2.nodes);
}

void delete_graph(graph_t *graph) {
	graph->size = 0;
	delete_linked_list(graph->nodes);
	free(graph);
}

result_t *graph_insert_node(graph_t *graph, bool isTerminal) {
	node_t *last = graph->last;
	node_t *node = new_node(graph->type, isTerminal, last == NULL ? 1 : last->id + 1);
	result_t *result = linked_list_push(graph->nodes, node);
	if(result->isSuccess) {
		graph->size++;
		graph->last = node;
	}
	return result;
}

result_t *graph_insert_edge(graph_t *graph, int id1, int id2, const char *weight) {
	Int *ptr_id1 = new_int(id1);
	result_t *result = linked_list_get(*graph->nodes, &ptr_id1->value);
	if(!result->isSuccess)
		return result;

	node_t *parent = (node_t*) result->value;
	pair_t *pair = new_pair(weight, new_mdb_type(&DB_INT, new_int(id2)));
	result = node_insert_child(parent, pair);
	return result;
}

result_t *graph_remove_node(graph_t *graph, int id) {
	Int *ptrId = new_int(id);
	result_t *result = linked_list_get(*graph->nodes, &ptrId->value);
	if(!result->isSuccess)
		return result;

	node_t *parent = (node_t*) result->value;
	node_clear_children(parent);
	graph->size--;
	return linked_list_delete(graph->nodes, ptrId);
}

result_t *graph_remove_edge(graph_t *graph, int id1, int id2) {
	Int *ptrId = new_int(id1);
	result_t *result = linked_list_get(*graph->nodes, &ptrId->value);
	if(!result->isSuccess)
		return result;
	node_t *parent = (node_t*) result->value;
	return node_remove_child_by_id(parent, id2);
}

result_t *graph_get_node(graph_t graph, int id) {
	return linked_list_get(*graph.nodes, &new_int(id)->value);
}

char *graph_to_string(const graph_t graph) {
	char *string = (char*) malloc(STRINGIFY_OBJECT_SIZE);
	int i = sprintf(string, "{"), x = 0;
	int size = graph.size;
	simple_node_t *reference = graph.nodes->begin;
	while(reference != NULL) {
		i += sprintf(string + i, "%s", node_to_string(*((node_t*)reference->value)));
		if(++x < size)
			i += sprintf(string + i, ", ");
		reference = reference->next;
	}
	string[i++] = '}';
	string[i] = '\0';
	return string;
}
