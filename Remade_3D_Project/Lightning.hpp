#ifndef LIGHTNING_HPP
#define LIGHTNING_HPP

class LineObject;

class Lightning
{
public:
	Lightning();
	~Lightning();



private:
	unsigned int m_nrOfLines;
	LineObject* m_lines;
};

#endif