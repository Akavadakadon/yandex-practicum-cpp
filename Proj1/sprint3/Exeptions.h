#pragma once

#include "SearchServer3.h"

void PrintDocument(const sprint3::Document& document) {
    cout << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s << endl;
}

void Task1()
{
    sprint3::SearchServer search_server("и в на"s);

    (void)search_server.AddDocument(1, "пушистый кот пушистый хвост"s, sprint3::DocumentStatus::ACTUAL, { 7, 2, 7 });

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, sprint3::DocumentStatus::ACTUAL, { 1, 2 })) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }

    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, sprint3::DocumentStatus::ACTUAL, { 1, 2 })) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }

    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, sprint3::DocumentStatus::ACTUAL, { 1, 3, 2 })) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }

    vector<sprint3::Document> documents;
    if (search_server.FindTopDocuments("a-a"s, documents)) {
        for (const sprint3::Document& document : documents) {
            PrintDocument(document);
        }
    }
    else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
}

void Task2()
{
    sprint3::SearchServer search_server("и в на"s);

    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void)search_server.AddDocument(1, "пушистый кот пушистый хвост"s, sprint3::DocumentStatus::ACTUAL, { 7, 2, 7 });

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, sprint3::DocumentStatus::ACTUAL, { 1, 2 })) {
        cout << "Документ не был добавлен, так его id совпадает с уже имеющимся"s << endl;
    }

    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, sprint3::DocumentStatus::ACTUAL, { 1, 2 })) {
        cout << "Документ не был добавлен, так его id отрицательный"s << endl;
    }

    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, sprint3::DocumentStatus::ACTUAL, { 1, 3, 2 })) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }

    if (const auto documents = search_server.FindTopDocuments("--пушистый"s)) {
        for (const sprint3::Document& document : *documents) {
            PrintDocument(document);
        }
    }
    else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
}

void Exeptions()
{
    cout << "\n\n Exeptions \n";
    cout << "\nTask1\n";
    Task1();
    cout << "\nTask2\n";
    Task2();


}



