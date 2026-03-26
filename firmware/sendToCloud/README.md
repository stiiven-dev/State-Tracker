# How sendToCloud works

```mermaid
flowchart LR
    A[setup] --> B{User exist?}
    B -->|Yes| C[Upload DATA]
    B -->|No| D[Creates row]
    D --> C
```
