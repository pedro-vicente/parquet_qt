# parquet_qt
Apache Parquet data file format display in Qt.
Parquet is an open source, column-oriented data file format designed for efficient data storage and retrieval.

## build parquet

```bash
git clone https://github.com/pedro-vicente/parquet_qt
cd parquet_qt
./build.parquet.cmake.sh
```
## build samples

```bash
./build.cmake.sh
```

## table 

```c++
auto parquet_table = arrow::Table::Make(
  arrow::schema({
      arrow::field("id", arrow::int32()),
      arrow::field("name", arrow::utf8())
    }),
  { id_arr, name_arr }
);
```


![Image](https://github.com/user-attachments/assets/ce716a8b-1852-4f07-840a-f3cff98b28d5)


