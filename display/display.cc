#include <iostream>
#include <memory>
#include <vector>

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>

#if defined(_MSC_VER)
#pragma warning(disable: 4834)
#endif 

///////////////////////////////////////////////////////////////////////////////////////
//MainWindow
///////////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
public:
  MainWindow()
  {
    ///////////////////////////////////////////////////////////////////////////////////////
    // make a parquet parquet_table 
    ///////////////////////////////////////////////////////////////////////////////////////

    arrow::Int32Builder ids;
    arrow::StringBuilder names;

    ids.AppendValues({ 1, 2, 3 });
    names.AppendValues({ "Alice", "Pedro", "Zahiah" });

    std::shared_ptr<arrow::Array> id_arr, name_arr;
    ids.Finish(&id_arr);
    names.Finish(&name_arr);

    auto parquet_table = arrow::Table::Make(
      arrow::schema({
          arrow::field("id", arrow::int32()),
          arrow::field("name", arrow::utf8())
        }),
      { id_arr, name_arr }
    );

    auto out = arrow::io::FileOutputStream::Open("table.parquet").ValueOrDie();
    parquet::arrow::WriteTable(*parquet_table, arrow::default_memory_pool(), out, 100);
    qDebug() << "Wrote table.parquet";

    auto in = arrow::io::ReadableFile::Open("table.parquet").ValueOrDie();
    auto reader = parquet::arrow::OpenFile(in, arrow::default_memory_pool()).ValueOrDie();

    std::shared_ptr<arrow::Table> result;
    reader->ReadTable(&result);
    std::cout << "Read " << result->num_rows() << " rows\n";

    ///////////////////////////////////////////////////////////////////////////////////////
    // transform the Parquet table into a QTableWidget
    ///////////////////////////////////////////////////////////////////////////////////////

    // central widget
    QWidget* container = new QWidget;

    // create horizontal layout 
    QVBoxLayout* layout = new QVBoxLayout;
    container->setLayout(layout);

    QTableWidget* table = new QTableWidget;
    table->setRowCount(parquet_table->num_rows());
    table->setColumnCount(parquet_table->num_columns());

    // set headers
    QStringList headers;
    for (int i = 0; i < parquet_table->num_columns(); ++i)
    {
      headers << QString::fromStdString(parquet_table->schema()->field(i)->name());
    }
    table->setHorizontalHeaderLabels(headers);

    // fill data
    for (int idx_col = 0; idx_col < parquet_table->num_columns(); ++idx_col)
    {
      auto column = parquet_table->column(idx_col)->chunk(0);
      for (int idx_row = 0; idx_row < parquet_table->num_rows(); ++idx_row)
      {
        QString value;
        if (column->type()->id() == arrow::Type::INT32)
        {
          auto int_array = std::static_pointer_cast<arrow::Int32Array>(column);
          value = QString::number(int_array->Value(idx_row));
        }
        else if (column->type()->id() == arrow::Type::STRING)
        {
          auto str_array = std::static_pointer_cast<arrow::StringArray>(column);
          value = QString::fromStdString(str_array->GetString(idx_row));
        }
        table->setItem(idx_row, idx_col, new QTableWidgetItem(value));
      }
    }

    table->resizeColumnsToContents();
    table->setWindowTitle("Parquet Viewer");
    table->show();

    layout->addWidget(table);

    setCentralWidget(container);
  }
};

///////////////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  MainWindow window;
  QSize screen = QGuiApplication::primaryScreen()->size();
  window.resize(screen.width(), screen.height());
  window.showMaximized();
  return app.exec();
}
