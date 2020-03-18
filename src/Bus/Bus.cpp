#include "Bus.h"

Bus::Bus(int id)
{
    id_ = id;
}

void
Bus::SetCurrentSquare(Square *square)
{
    currentSquare = square;
}

void
Bus::CreateTimetable(QPlainTextEdit *text)
{
    Coordinates *coordinates;
    std::ifstream file;
    std::string line;
    std::vector<std::string> tokens;

    /* get file path */
    std::ostringstream ss;
    ss << "../files/bus-route-map/" << std::to_string(id_) << ".txt";
    std::string path = Functions::GetAbsolutePath(ss.str().c_str());

    file.open(path);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");
        coordinates = Stop::GetStop(tokens[1]);
        if (!coordinates) {
            std::cerr << "Error: bus stop -- " << tokens[1] << " -- wasn't found" << std::endl;
        } else {
            stopCoordinates.push_back(coordinates);
        }
        text->appendPlainText(QString::fromStdString(line));
        text->moveCursor (QTextCursor::End);
    }

    file.close();
}
