#include <QCoreApplication>
#include <QLabel>
#include <QString>
#include <QtTest>
#include <memory>

#include "QtReactive.hpp"

class Reactive : public QObject {
  Q_OBJECT

public:
  Reactive() {}
  ~Reactive() {}

private slots:
  void test_label() {
    QLabel label;
    Ref<QString> text("1");

    // insecure access to label
    reactive_watch(&label, [&] { label.setText(text); });
    QCOMPARE(text.value(), "1");
    QCOMPARE(label.text(), "1");

    text = "2";
    QCOMPARE(text.value(), "2");
    QCOMPARE(label.text(), "2");

    text = "3";
    QCOMPARE(text.value(), "3");
    QCOMPARE(label.text(), "3");
  }

  void test_label_multiple_watchers() {
    QLabel label;
    Ref<QString> text("1");

    // insecure access to label
    reactive_watch(&label, [&] { label.setText(text); });
    reactive_watch(&label, [&] { label.setToolTip(text + "ttp"); });
    QCOMPARE(text.value(), "1");
    QCOMPARE(label.text(), "1");
    QCOMPARE(label.toolTip(), "1ttp");

    text = "2";
    QCOMPARE(text.value(), "2");
    QCOMPARE(label.text(), "2");
    QCOMPARE(label.toolTip(), "2ttp");
  }

  void test_label_destroy() {
    Ref<QString> text("1");

    {
      QLabel label;
      reactive_watch(&label, [&] { label.setText(text); });
    }

    text = "2";
    QCOMPARE(text.value(), "2");
  }

  void test_ref_destroy() {
    auto text = std::make_unique<Ref<QString>>("1");

    QLabel label;
    reactive_watch(&label, [&] { label.setText(*text); });

    text.reset();
  }
};

QTEST_MAIN(Reactive)

#include "tst_reactive.moc"
