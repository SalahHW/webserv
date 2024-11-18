#pragma once

#include "Directive.hpp"

class RootDirective : public Directive {
public:
	~RootDirective();
	RootDirective(Block* currentContext, const std::string& fullDirectiveLine);
	RootDirective(const RootDirective& other);
	RootDirective& operator=(const RootDirective& other);

	virtual bool validateSpecific();
	virtual void displayInfo() const;
	virtual void apply(Location& location);

	void setRootPath(const std::string& rootPath);
	const std::string& getRootPath() const;
	
private:
	RootDirective();

	//TODO: Check if this is the correct type for rootPath
	std::string rootPath;
};