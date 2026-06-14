#pragma once
#include <src/DataReaders/SqliteReader.h>
#include <src/DataReaders/DataReaderManager.h>

REGISTER_DATA_READER("sqlite", SqliteReader)
