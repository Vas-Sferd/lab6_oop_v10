#include "pch.h"

using namespace System;
using namespace System::Xml;

class Landscape {
    int square;
public:
    Landscape(int square) : square(square) {}
    int getSquare() {
        return square;
    }
};

class NaturalLandscape : public Landscape {
public:
    NaturalLandscape(int square) : Landscape(square) {}
};

class HasaAverageTemperature {
    int temperature;
public:
    HasaAverageTemperature(int temperature) : temperature(temperature) {}
    int getTemperature() {
        return temperature;
    }
};

class Forest : public NaturalLandscape, public HasaAverageTemperature {
public:
    Forest(int square, int temperature) : NaturalLandscape(square), HasaAverageTemperature(temperature) {}
};

ref class Store {
    std::vector<Forest>* data;
    XmlTextReader^ reader;
public:
    Store() : data(new std::vector<Forest>), reader(gcnew XmlTextReader("data.xml")) {}
    ~Store() {
        delete data;
    }

    void addForest(Forest forest) {
        data->push_back(forest);
    }

    void deleteForest(int position) {
        data->erase(data->begin() + position - 1);
    }

    void load() {
        try {
            while (reader->Read()) {
                int square = 0;
                int temperature = 0;
                switch (reader->NodeType) {
                case XmlNodeType::Element:
                    while (reader->MoveToNextAttribute()) {
                        int value = Int32::Parse(reader->Value);
                        if (reader->Name == "temperature") {
                            temperature = value;
                        } else if (reader->Name == "square") {
                            square = value;
                        }
                    }
                    break;
                }
                addForest(Forest(square, temperature));
            }
        } catch (...) {
        }
    }
    void save() {
        System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter("data.xml");
        for (Forest f : *data) {
            sw->WriteLine("<Forest square='" + f.getSquare() + "' temperature" + f.getTemperature() + "'>");
        }
    }
    void print() {
        for (Forest f : *data) {
            Console::WriteLine("<Forest square='" + f.getSquare() + "' temperature='" + f.getTemperature() + "'>");
        }
    }
};

int main(array<System::String ^> ^args)
{
    Store store;
    Forest f1 = {150, 26};
    Forest f2 = {142, 32};
    Forest f3 = {120, 15};

    store.addForest(f1);
    Console::WriteLine("Добавлено одно значение");
    store.print();

    store.addForest(f2);
    store.addForest(f3);
    store.save();
    Console::WriteLine("Добавлено пару значений и сохранено в файл");
    store.print();

    store.deleteForest(2);
    Console::WriteLine("Удаление второго элемента");
    store.print();

    store.load();
    Console::WriteLine("Восстановление значения из файла");
    store.print();

    return 0;
}
