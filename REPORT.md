# Report: Building a search engine like Google

# Diagrama del Sistema

## Documents
- **Document**
  - `document_deserialize`

## Lineal
- **Query Node**
  - `searchDocumentLineal`
  - `Query_init`

## Hashmap
- **DocIdList**
  - `DocIdList_create`
  - `DocIdList_add`
- **HashNode**
- Funcions:
  - `HashMap_create`
  - `add_words_to_reverse_index`

## Graph
- **Graph**
  - `crear_graph`
- **Relevance**
  - `relevance_score_filtered`

