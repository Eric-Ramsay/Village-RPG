#pragma once

void processMessages() {
	for (;;) {
		Sleep(1);
		while (firstProcess != nullptr) {
			static int numMessages = 0;
			std::string data = firstProcess->data;
			std::string type = firstProcess->type;
			std::cout << type << " " << std::endl;
			if (type == "TEXT") {
				logs.push_back(data);
			}
			if (type == "READY") {
				char myhostname[256];
				int rc = gethostname(myhostname, sizeof myhostname);
				std::cout << myhostname << std::endl;
				ID = myhostname;
				sendData("LOG_IN", std::string(myhostname));
			}
			if (type == "CHARACTER") {
				Character C;
				std::vector<std::string> strings = split(data, '\n');
				for (std::string str : strings) {
					std::string stat = readStr(str);
					characterChange(C, stat, str);
				}
				CHARACTERS[C.ID] = C;
			}
			if (type == "STAT") {
				std::string id = readStr(data);
				std::string stat = readStr(data);
				characterChange(CHARACTERS[id], stat, data);
			}
			if (type == "BATTLE") {

			}
			Message* temp = firstProcess;
			firstProcess = firstProcess->next;
			delete temp;
		}
		if (bufferStart != nullptr && !SENDING) {
			firstProcess = bufferStart;
			bufferStart = nullptr;
			bufferEnd = nullptr;
		}
	}
}