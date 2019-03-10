#include <simple_label.h>

#include <drawing_manager.h>

void SimpleLabel::Render() {
	int offset = 0;
	bool endOfString = false;

	for (int i = 0; i < m_totalChars; i++) {
		if (!endOfString && m_text[i]) {
			if (m_text[i] <= ':' && m_text[i] >= '0') {
				offset = (m_text[i] - '0') * 36;
			}
		}
		else {
			offset = 11 * 36;
			endOfString = true;
		}

		DRAWING_MANAGER.DrawImageCropped(m_textSymbols, m_location + Vector2(i * 36, 0), Vector2(offset, 0), Vector2(36, 32));
	}
}