graph TD

%% Documents
Document --> document_deserialize

%% Lineal
QueryNode --> searchDocumentLineal
QueryNode --> Query_init

%% Hashmap
DocIdList --> DocIdList_create
DocIdList --> DocIdList_add
Hashmap --> DocIdList
Hashmap --> HashNode
HashNode --> HashMap_create
HashNode --> add_words_to_reverse_index

%% Graph
Graph --> crear_graph
Relevance --> relevance_score_filtered