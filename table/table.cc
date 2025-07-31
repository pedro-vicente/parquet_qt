#include <iostream>
#include <memory>
#include <vector>

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#if defined(_MSC_VER)
#pragma warning(disable: 4834)
#endif 

///////////////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////////////

int main()
{
  arrow::Int32Builder ids;
  arrow::StringBuilder names;

  ids.AppendValues({ 1, 2, 3 });
  names.AppendValues({ "Alice", "Pedro", "Zahiah" });

  std::shared_ptr<arrow::Array> id_arr, name_arr;
  ids.Finish(&id_arr);
  names.Finish(&name_arr);

  auto table = arrow::Table::Make(
    arrow::schema({
        arrow::field("id", arrow::int32()),
        arrow::field("name", arrow::utf8())
      }),
    { id_arr, name_arr }
  );

  auto out = arrow::io::FileOutputStream::Open("table.parquet").ValueOrDie();
  parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), out, 100);
  std::cout << "Wrote table.parquet\n";

  auto in = arrow::io::ReadableFile::Open("table.parquet").ValueOrDie();
  auto reader = parquet::arrow::OpenFile(in, arrow::default_memory_pool()).ValueOrDie();

  std::shared_ptr<arrow::Table> result;
  reader->ReadTable(&result);
  std::cout << "Read " << result->num_rows() << " rows\n";
}
