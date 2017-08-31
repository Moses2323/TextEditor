#include "filetotabwidget.h"

//------------------ OnlyDoubleDelegate

QWidget* fttw::OnlyDoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
	QLineEdit *lineEdit = new QLineEdit(parent);
	QDoubleValidator *validator = new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 15, lineEdit);
	validator->setLocale(QLocale(QLocale::English));
	lineEdit->setValidator(validator);
	return lineEdit;
}


//------------------ HighLightNumbers

void fttw::HighLightNumbers::init(){
	intFormat_.setForeground(Qt::darkRed);
	doubleFormat_.setForeground(Qt::red);
	keywordFormat_.setForeground(Qt::blue);

	HighlightRule rule;

	QStringList keyWords;
	keyWords<<"^" + TablePrintElem::trigger + "\\b"
	        <<"^" + TablePrintElem::triggerName + "\\b"
	        <<"^" + VectorsPrintElem::trigger + "\\b"
	        <<"^" + VectorsPrintElem::triggername + "\\b"
	        <<"^" + VectorPrintElem::trigger + "\\b"
	        <<"^" + GlobalValuePrintElem::trigger + "\\b"
	        <<"^" + GlobalValuesPrintElem::trigger + "\\b";

	for(QStringList::iterator it = keyWords.begin(); it != keyWords.end(); ++it){
		rule.format = keywordFormat_;
		rule.pattern = QRegExp(*it);
		rules_.push_back(rule);
	}

	rule.format = intFormat_;
	rule.pattern = QRegExp("[\\+\\-]?\\b[0-9][0-9]*\\b");
	rules_.push_back(rule);

	QStringList doublePatterns;
	doublePatterns<<"[\\+\\-]?\\b[0-9]*\\.[0-9]*\\b"
				  <<"[\\+\\-]?\\b[0-9]*\\.?[0-9][eE][\\-\\+]?[0-9][0-9]*\\b";
	for(QStringList::iterator it = doublePatterns.begin(); it != doublePatterns.end(); ++it){
		rule.format = doubleFormat_;
		rule.pattern = QRegExp(*it);
		rules_.push_back(rule);
	}
}

void fttw::HighLightNumbers::highlightBlock(const QString &str){
	int index;
	int L;
	for(int i = 0; i < rules_.size(); ++i){
		QRegExp expression(rules_[i].pattern);
		index = expression.indexIn(str);
		while(index >= 0){
			L = expression.matchedLength();
			setFormat(index, L, rules_[i].format);
			index = expression.indexIn(str, index + L);
		}
	}
}


//------------------ FileToTabWidget

std::ostream& fttw::FileToTabWidget::print(std::ostream& s) const{
	s<<"FileToTabWidget:"<<std::endl;
	s<<"  name = "<<objectName().toStdString()<<std::endl;
	s<<"  ";
	print_parent(s, this);
	s<<std::endl<<std::endl;

	s<<"  mainWidget = "<<mW_->objectName().toStdString()<<std::endl;
	s<<"  parent of mainWidget = ";
	print_parent(s, mW_);
	s<<std::endl;
	s<<"  mainLayout = ";
	tryToPrint(s, mL_);
	s<<std::endl;

	s<<"  unmarkedValues_ = ";
	tryToPrint(s, unmarkedValues_);
	s<<std::endl<<std::endl;

	s<<"  tabsWidget = ";
	tryToPrint(s, tabs_);
	s<<std::endl<<std::endl;

	s<<"  all globalValues (N = "<<globalValues_.size()<<"):"<<std::endl;
	for(size_t i=0; i<globalValues_.size(); ++i){
		s<<"    glV["<<i<<"].first = ";
		tryToPrint(s, globalValues_[i].first);
		s<<std::endl;
		s<<"    glV["<<i<<"].second = ";
		tryToPrint(s, globalValues_[i].second);
		s<<std::endl;
	}
	s<<std::endl;

	s<<"  all tables (N = "<<tables_.size()<<"):"<<std::endl;
	for(size_t i=0; i<tables_.size(); ++i){
		s<<"    tb["<<i<<"] = ";
		tryToPrint(s, tables_[i]);
		s<<std::endl;
	}
	s<<std::endl;
	return s;
}

void fttw::FileToTabWidget::layout_init(){
	mL_ = new QVBoxLayout;
	mW_->setLayout(mL_);
	mL_->setObjectName("mainLayout");
}

void fttw::FileToTabWidget::main_init(){
	setObjectName("ScrollArea");
	mW_ = new QFrame;

	mW_->setObjectName("mainWidget");
	layout_init();
	setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	setWidget(mW_);
	setWidgetResizable(true);
}

void fttw::FileToTabWidget::createFileBuffer(const std::string &filename, std::ostream& s){
	std::ifstream fin;
	fin.open(filename.c_str(), std::ios::in);
	if (!fin.is_open()){
		fttw::print_mistake(toStr(__FUNCTION__)+"(const string&, stringstream&)", __LINE__, __FILE__, "can\'t open file with name\n"+toStr(filename));
	}

	std::string vs = "";
	getline(fin,vs);
	while(!fin.eof()){
		s<<vs<<'\n';
		getline(fin, vs);
	}
	s<<vs<<'\n';

	fin.close();
}

void fttw::FileToTabWidget::createTextEditor(std::stringstream& vss){
	std::cout<<"CREATING TEXT EDITOR..."<<std::endl;

	unmarkedValues_ = new QTextEdit(mW_);
	unmarkedValues_->setObjectName("text_editor");

	unmarkedValues_->setText(QString::fromStdString(vss.str()));
	unmarkedValues_->setTabStopWidth(30);

	textEditorHighLighter_ = new HighLightNumbers(unmarkedValues_);
	textEditorHighLighter_->setObjectName("textEditor_highlighter");

	mL_->addWidget(unmarkedValues_);

	std::cout<<"TEXT EDITOR WAS CREATED"<<std::endl;
}

void fttw::FileToTabWidget::createLabelVector(const std::string &labels, std::vector<std::string> &vec) const{
	std::stringstream vss;		//поток для чтения названий глобальных параметров в этой строчке
	vss.str(labels);
	vec.reserve(5);
	std::string vs;
	while(vss>>vs){			//считываем все названия и производим подсчет количества параметров в данной строке
		vec.push_back(vs);
	}
}

size_t fttw::FileToTabWidget::readGlobalValues(std::istream& ss, const std::string &labelLine, int& rowCounter, int& columnCounter){
	std::vector<std::string> labels;		//названия глобальных параметров
	createLabelVector(labelLine, labels);
	std::vector<std::string> values;		//считанные значения глобальных параметров
	values.reserve(labels.size());

	size_t N = labels.size();	//кол-во глобальных параметров, которое требуется
							//считываем значения параметров
	std::string vs;
	for(size_t i=0; i<N; ++i){
		vs = "";
		ss>>vs;
		values.push_back(vs);
	}
							//создаем метку и текстовое поле для каждого из параметров
	QLabel *oneLabel;
	QLineEdit *oneLineEdit;
	QHBoxLayout *oneLayout;
	QDoubleValidator *validator;
	QString valName;
	globalValues_.reserve(N);

	for(size_t i=0; i<N; ++i){
		valName = QString::fromStdString(labels[i]);
		oneLayout = new QHBoxLayout;
		oneLayout->setObjectName("layoutFor_"+valName);
		oneLabel = new QLabel(valName);
		oneLabel->setObjectName("labelFor_"+valName);
		oneLineEdit = new QLineEdit(QString::fromStdString(values[i]));
		oneLineEdit->setObjectName("lineEditFor_"+valName);

		validator = new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 15, oneLineEdit);
		validator->setObjectName("validatorFor_"+valName);
		validator->setLocale(QLocale::English);
		oneLineEdit->setValidator(validator);

		oneLayout->addWidget(oneLabel);
		oneLayout->addWidget(oneLineEdit);

		widgetWithLabelsLayout_->addLayout(oneLayout, rowCounter, columnCounter);
		std::cout<<"   rowCounter =  "<<rowCounter <<", columnCounter = "<<columnCounter<<std::endl;

		if (columnCounter == 1){
			columnCounter = 0;
			++rowCounter;
		}
		else{
			columnCounter = 1;
		}
		globalValues_.push_back(std::pair<QLabel*, QLineEdit*>(oneLabel, oneLineEdit));
	}
	return N;
}

void fttw::FileToTabWidget::addTableToTab(QTableWidget *table){
	if (tabs_ == nullptr){
		tabs_ = new QTabWidget(mW_);
		tabs_->setObjectName("tabsArray");
	}
	tabs_->addTab(table, QString::number( tables_.size()+vectors_.size() ));
	table->setObjectName("table_"+QString::number( tables_.size()+vectors_.size() ));
}

void fttw::FileToTabWidget::addTableToTab(QTableWidget *table, const QString &tabname){
	if (tabs_ == nullptr){
		tabs_ = new QTabWidget(mW_);
		tabs_->setObjectName("tabsOrVecArray");
	}
	tabs_->addTab(table, tabname);
	table->setObjectName("tableOrVec_"+tabname);
}



size_t fttw::FileToTabWidget::readVector(std::istream &ss, const std::string &labels){
	std::vector<std::string> vecLabels;
	createLabelVector(labels, vecLabels);
	size_t Nvec = vecLabels.size();

	QStringList listLables;
	for(size_t i=0; i<Nvec; ++i){
		listLables<<QString::fromStdString(vecLabels[i]);
	}

	std::vector<std::string> data;
	data.reserve(Nvec*100);
	std::string vs;
	vs.reserve(10);
	size_t NnumInOneVec = 0;

	size_t numberOfEntersAfterTable = 0;
	char cheker = 0;
	while(ss>>vs){
		for(size_t i=0; i<vs.size(); ++i){
			ss.unget();
		}
		if (isDouble(vs)){
			data.push_back(vs);
			ss>>vs;
		}
		else {break;}
		vs = "";

		ss>>std::noskipws;
		numberOfEntersAfterTable = 0;
		while(ss.peek() == '\n'){
			ss>>cheker;
			++numberOfEntersAfterTable;
		}
		ss>>std::skipws;
	}
	if (numberOfEntersAfterTable != 0) {--numberOfEntersAfterTable;}

	NnumInOneVec = data.size() / Nvec;

	QTableWidget* vec = new QTableWidget(Nvec, NnumInOneVec, mW_);
	vec->setItemDelegate(new OnlyDoubleDelegate);
	vec->setVerticalHeaderLabels(listLables);

	listLables.clear();
	for(size_t i=0; i<NnumInOneVec; ++i){
		listLables<<QString::number(i+1);
	}
	vec->setHorizontalHeaderLabels(listLables);

	QTableWidgetItem* item;
	for(size_t i=0; i<Nvec; ++i){
		for(size_t k=0; k<NnumInOneVec; ++k){
			item = new QTableWidgetItem( QString::fromStdString( data[k + i*NnumInOneVec] ) );
			vec->setItem(i,k, item);
		}
	}
	vectors_.push_back(vec);
	return numberOfEntersAfterTable;
}

size_t fttw::FileToTabWidget::readTable(std::istream &ss, const std::string &labels){
	std::vector<std::string> vecLabels;				//названия столбцов
	createLabelVector(labels, vecLabels);
	size_t Ncol = vecLabels.size();			//количество столбцов

	QStringList listLabels;					//создаем QStringList из вектора
	for(size_t i=0; i<Ncol; ++i){
		listLabels<<QString::fromStdString(vecLabels[i]);
	}

	std::vector<std::vector<std::string> > data;			//данные из файла, которые будут изначально в таблице
	std::string vs;
	vs.reserve(10);
	size_t Nrow = 0;

	size_t numberOfEntersAfterTable = 0;
	char cheker = 0;
	while(ss>>vs){							//считывание данных
		for(size_t i=0; i<vs.size(); ++i){
			ss.unget();
		}
		if (isDouble(vs)){
			Nrow = data.size();
			data.resize(Nrow+1);
			++Nrow;
			data[Nrow-1].resize(Ncol);
			for(size_t i=0; i<Ncol; ++i){
				vs = "";
				ss>>vs;
				data[Nrow-1][i] = vs;
			}
		}
		else {
			break;
		}
		vs = "";

		ss>>std::noskipws;
		numberOfEntersAfterTable = 0;
		while(ss.peek() == '\n'){
			ss>>cheker;
			++numberOfEntersAfterTable;
		}
		ss>>std::skipws;
	}
	if (numberOfEntersAfterTable != 0) {--numberOfEntersAfterTable;}
	Nrow = data.size();

	QTableWidget* table = new QTableWidget(Nrow, Ncol, mW_);
	table->setItemDelegate(new OnlyDoubleDelegate);
	table->setHorizontalHeaderLabels(listLabels);

	listLabels.clear();
	for(size_t i=0; i<Nrow; ++i){
		listLabels<<QString::number(i+1);
	}
	table->setVerticalHeaderLabels(listLabels);

	QTableWidgetItem* item;
	for(size_t i=0; i<Nrow; ++i){
		for(size_t k=0; k<Ncol; ++k){
			item = new QTableWidgetItem(QString::fromStdString(data[i][k]));
			table->setItem(i,k, item);
		}
	}

	tables_.push_back(table);
	return numberOfEntersAfterTable;
}

QString fttw::FileToTabWidget::getTabName(std::istream &ss) const{
	std::string vs;
	getOneLine(ss, vs);
	QString res;
	res = QString::fromStdString(vs);

	if (vs.size() == 0){
		print_mistakeLite(toStr(__FUNCTION__)+"(istream&)", __LINE__, __FILE__, "tab or vec name is empty");
	}

	return res;
}

bool fttw::FileToTabWidget::checkGlobalValues(std::istream &ss, const std::string &labels) const{
	std::stringstream vss;		//поток для чтения названий глобальных параметров в этой строчке
	vss.str(labels);
	std::string vs;
	size_t N = 0;
	while(vss>>vs){			//считываем все названия и производим подсчет количества параметров в данной строке
		++N;
	}

	for(size_t i=0; i<N; ++i){
		vs = "";
		ss>>vs;
		if ((vs.size() == 0) || !isDouble(vs)){
			print_mistakeLite(toStr(__FUNCTION__)+"(stringstream&, const string&)", __LINE__, __FILE__, "too few values for defining all global parameters. vs=\""+toStr(vs)+"\" is not a double.");
			return false;
		}
	}
	return true;
}

bool fttw::FileToTabWidget::checkVector(std::istream &ss, const std::string &labels) const{
	std::stringstream vss;
	vss.str(labels);
	std::string vs;

	size_t Nvec = 0;
	while(vss>>vs)
		++Nvec;
	if (Nvec == 0)
		return false;

	vs = "";
	int NnumInOneVec = 0;
	while(ss>>vs){			//считывание данных
		for(size_t k=0; k<vs.size(); ++k)
			ss.unget();
		if (isDouble(vs)){
			++NnumInOneVec;
			ss>>vs;
		}
		else
			break;
		vs = "";
	}
	if (NnumInOneVec % Nvec == 0)
		return true;
	else
		return false;
}

bool fttw::FileToTabWidget::checkTable(std::istream &ss, const std::string &labels) const{
	std::stringstream vss;		//поток для чтения названий глобальных параметров в этой строчке
	vss.str(labels);
	std::string vs;

	size_t Ncol = 0;
	while(vss>>vs)			//считываем все названия и производим подсчет количества параметров в данной строке
		++Ncol;
	if (Ncol == 0)
		return false;

	vs = "";
	while(ss>>vs){							//считывание данных
		for(size_t i=0; i<vs.size(); ++i)
			ss.unget();
		if (isDouble(vs)){
			for(size_t i=0; i<Ncol; ++i){
				vs = "";
				ss>>vs;
				if ((vs.size() == 0) || !isDouble(vs))
					return false;
			}
		}
		else
			break;
		vs = "";
	}

	return true;
}

bool fttw::FileToTabWidget::check_correct(std::istream& vss) const{
	std::cout<<"CHECKING FILE FOR CORRECT INPUT..."<<std::endl;

	bool res = false;

	std::string vsLine;
	QString vs;

	vss>>vsLine;
	while(!vss.eof()){
		if (!vsLine.compare(0, GlobalValuePrintElem::trigger.size(), GlobalValuePrintElem::trigger.toStdString(), 0, GlobalValuePrintElem::trigger.size())){
			getOneLine(vss, vsLine);
			res = checkGlobalValues(vss, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "global values writed incorrectly");
				break;
			}
		}
		else if(!vsLine.compare(0, VectorsPrintElem::triggername.size(), VectorsPrintElem::triggername.toStdString(), 0, VectorsPrintElem::triggername.size())){
			vs = getTabName(vss);
			vss>>vsLine;

			getOneLine(vss, vsLine);
			res = checkVector(vss, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "One of your vectors was written incorrectly");
				break;
			}
		}
		else if( (!vsLine.compare(0, VectorPrintElem::trigger.size(), VectorPrintElem::trigger.toStdString(), 0, VectorPrintElem::trigger.size())) || (!vsLine.compare(0, 6, "#vector", 0, 6)) ){
			getOneLine(vss, vsLine);
			res = checkVector(vss, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "One of your vectors was written incorrectly");
				break;
			}
		}
		else if(!vsLine.compare(0, TablePrintElem::triggerName.size(), TablePrintElem::triggerName.toStdString(), 0, TablePrintElem::triggerName.size())){	//таблица вместе с именем вкладки
			vs = getTabName(vss);
			vss>>vsLine;

			getOneLine(vss, vsLine);
			res = checkTable(vss, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "One of your tables was written incorrectly");
				break;
			}
		}
		else if(!vsLine.compare(0, TablePrintElem::trigger.size(), TablePrintElem::trigger.toStdString(), 0, TablePrintElem::trigger.size())){		//таблица
			getOneLine(vss, vsLine);
			res = checkTable(vss, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "One of your tables was written incorrectly");
				break;
			}
		}
		else if(vsLine[0] == CommentElement::trigger){							//комментарий
			getOneLine(vss, vsLine);
		}
		else if (vsLine.size() > 0){						//неразмеченная строка
			res = false;
			break;
		}
		vss>>vsLine;
	}

	std::cout<<"CHECKING IS COMPLITE"<<std::endl;
	return res;
}

void fttw::FileToTabWidget::createTabsAndGlobalValues(std::istream& fin){
	std::cout<<"START LOADING FILE..."<<std::endl;

	tabsFromLabelsSplitter_ = new QSplitter( Qt::Orientation::Vertical );
	labelsScrollArea_ = new QScrollArea;

	mL_->addWidget( tabsFromLabelsSplitter_ );
	tabsFromLabelsSplitter_->addWidget( labelsScrollArea_ );

	widgetWithLabels_ = new QWidget;
	widgetWithLabelsLayout_ = new QGridLayout;

	char c='a';

	size_t numberOfEnters = 0;
	bool flagComment = false;
	size_t numberOfGlobalValuesInOneString = 0;

	int rowCounter = 0;
	int columnCounter = 0;

	std::string vsLine;
	QString vs;
	fin>>vsLine;
	while(!fin.eof()){
		if (!vsLine.compare(0, GlobalValuePrintElem::trigger.size(), GlobalValuePrintElem::trigger.toStdString(), 0, GlobalValuePrintElem::trigger.size())){				//глобальные параметры
			getOneLine(fin, vsLine);
			numberOfGlobalValuesInOneString = readGlobalValues(fin, vsLine, rowCounter, columnCounter);
			if (numberOfGlobalValuesInOneString == 1)
				printElements_.push_back(new GlobalValuePrintElem(globalValues_[globalValues_.size()-1].first, globalValues_[globalValues_.size()-1].second));
			else{
				GlobalValuesPrintElem* gvpe = new GlobalValuesPrintElem;
				for(size_t i = globalValues_.size() - numberOfGlobalValuesInOneString; i<globalValues_.size(); ++i){
					gvpe->addLabel(globalValues_[i].first);
					gvpe->addEditor(globalValues_[i].second);
				}

				printElements_.push_back(gvpe);
			}
		}
		else if (!vsLine.compare(0, VectorsPrintElem::triggername.size(), VectorsPrintElem::triggername.toStdString(), 0, VectorsPrintElem::triggername.size())){ //вектора вместе с именем вкладки
			vs = getTabName(fin);
			VectorsPrintElem* vpe = new VectorsPrintElem;
			vpe->setName(vs);

			fin>>vsLine;

			getOneLine(fin, vsLine);
			numberOfEnters = readVector(fin, vsLine);
			std::cout<<"vectors with names were read. NoE = "<<numberOfEnters<<std::endl;
			addTableToTab(vectors_[vectors_.size()-1], vs);
			vpe->setVectors(vectors_[vectors_.size()-1]);

			printElements_.push_back(vpe);
		}
		else if (!vsLine.compare(0, VectorsPrintElem::trigger.size(), VectorsPrintElem::trigger.toStdString(), 0, VectorsPrintElem::trigger.size())){		//вектора
			getOneLine(fin, vsLine);
			numberOfEnters = readVector(fin, vsLine);
			std::cout<<"vectors without names were read. NoE = "<<numberOfEnters<<std::endl;
			addTableToTab(vectors_[vectors_.size()-1]);

			printElements_.push_back(new VectorsPrintElem(vectors_[vectors_.size()-1]));
		}
		else if (!vsLine.compare(0, VectorPrintElem::trigger.size(), VectorPrintElem::trigger.toStdString(), 0, VectorPrintElem::trigger.size())){
			getOneLine(fin, vsLine);
			VectorPrintElem* vpe = new VectorPrintElem;

			numberOfEnters = readVector(fin, vsLine);
			std::cout<<"vector was read. NoE = "<<numberOfEnters<<std::endl;
			vpe->setVector(vectors_[vectors_.size()-1]);
			addTableToTab(vectors_[vectors_.size()-1], vpe->getName());

			printElements_.push_back(vpe);
		}
		else if(!vsLine.compare(0, TablePrintElem::triggerName.size(), TablePrintElem::triggerName.toStdString(), 0, TablePrintElem::triggerName.size())){		//таблица вместе с именем вкладки
			vs = getTabName(fin);
			TablePrintElem* tpe = new TablePrintElem;
			tpe->setName(vs);

			fin>>vsLine;

			getOneLine(fin, vsLine);
			numberOfEnters = readTable(fin, vsLine);
			std::cout<<"table with name was read. NoE = "<<numberOfEnters<<std::endl;
			addTableToTab(tables_[tables_.size()-1], vs);
			tpe->setTable(tables_[tables_.size()-1]);

			printElements_.push_back(tpe);
		}
		else if(!vsLine.compare(0, TablePrintElem::trigger.size(), TablePrintElem::trigger.toStdString(), 0, TablePrintElem::trigger.size())){			//таблица
			getOneLine(fin, vsLine);
			numberOfEnters = readTable(fin, vsLine);
			std::cout<<"table without name were read. NoE = "<<numberOfEnters<<std::endl;
			addTableToTab(tables_[tables_.size()-1]);

			printElements_.push_back(new TablePrintElem(tables_[tables_.size()-1]));
		}
		else if(vsLine[0] == CommentElement::trigger){																									//комментарий
			std::string oldvs = vsLine;
			getOneLine(fin, vsLine);
			oldvs += vsLine;

			printElements_.push_back(new CommentElement(oldvs));
			flagComment = true;
		}
		else if (vsLine.size() > 0){																													//неразмеченная строка
			print_mistake(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "check function for checking - they miss something, because file have unmarked string");
		}

		if (numberOfEnters > 0){
			for(size_t i=0; i<numberOfEnters; ++i){
				printElements_.push_back(new CommentElement);
			}
			numberOfEnters = 0;
		}

		fin>>std::noskipws;
		while(fin.peek() == '\n'){
			fin>>c;
			++numberOfEnters;
			std::cout<<"creating new enter in while"<<std::endl;
		}
		fin>>std::skipws;

		if (numberOfEnters > 0){
			if (!flagComment) {--numberOfEnters;}	//если не комментарий
			for(size_t i=0; i<numberOfEnters; ++i){	//если комментарий
				printElements_.push_back(new CommentElement);
				std::cout<<"creating new enter"<<std::endl;
			}
			numberOfEnters = 0;
		}

		vsLine = "";
		fin>>vsLine;
		flagComment = false;
	}

	printElements_.pop_back();

	if (tabs_ != nullptr){
		tabsFromLabelsSplitter_->addWidget(tabs_);
	}

//	setWidthFromTables();

	widgetWithLabels_->setLayout( widgetWithLabelsLayout_ );
	labelsScrollArea_->setWidget( widgetWithLabels_ );

	std::cout<<"LOADING FILE HAS COMPLETED"<<std::endl;
}

void fttw::FileToTabWidget::chooseWhatCreate(){
	bool correct = check_correct(fttwSS_);
	fttwSS_.clear();
	fttwSS_.seekg(std::ios_base::beg);

	if ((mode_ == mode::Tabs) && correct){
		createTabsAndGlobalValues(fttwSS_);
	}
	else{
		createTextEditor(fttwSS_);
		mode_ = mode::TextEditor;
	}

	loaderWidget.remove();
}

inline void fttw::FileToTabWidget::load_file(const std::string &filename){
	fttwSS_.clear();
	fttwSS_.str("");
	fttwFBthread_.reset( new FTTWFileBufferThread(filename, fttwSS_) );
	connect(fttwFBthread_.get(), SIGNAL(finishedReading()), this, SLOT(chooseWhatCreate()));

	loaderWidget.install(this);

	fttwFBthread_->start();
}

fttw::FileToTabWidget::FileToTabWidget(const std::string &filename, QWidget *pwg, mode in_mode) : QScrollArea(pwg), mode_(in_mode){
	main_init();
	load_file(filename);
}

void fttw::FileToTabWidget::saveFromData(std::ostream& fout) const{
	for(size_t i=0; i<printElements_.size(); ++i){
		printElements_[i]->printElem(fout);
	}
}

void fttw::FileToTabWidget::saveFromEditor(std::ostream& fout) const{
	std::string s = unmarkedValues_->toPlainText().toStdString();
	int Ne = 0;
	for(int i = s.size()-1; i>=0; --i){
		if (s[i] == '\n'){
			++Ne;
		}
		else {break;}
	}
	s = s.substr(0, s.size() - Ne);

	fout << s << std::endl;
}

void fttw::FileToTabWidget::save_file(const std::string &filename) const{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios::out);
	if (!fout.is_open()){
		print_mistake(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "can\'t create or edit file with name\n"+filename);
	}

	if (unmarkedValues_ == nullptr){
		std::cout<<"SAVING FROM DATA..."<<std::endl;
		saveFromData(fout);
	}
	else{
		std::cout<<"SAVING FROM EDITOR..."<<std::endl;
		saveFromEditor(fout);
	}
	std::cout<<"SAVING COMPLITE"<<std::endl;

	fout.close();
}


//----------------------- OutSideWidget

void fttw::OutSideWidget::savedata_slot(){
	emit savedata(savename_);
}

void fttw::OutSideWidget::hideSomeButtons(){
	if (fttw_->getMode() == FileToTabWidget::mode::Tabs){
		toTextEditorButton_->show();
		toTabsAndTablesButton_->hide();
	}
	else{
		toTextEditorButton_->hide();
		toTabsAndTablesButton_->show();
	}
}

void fttw::OutSideWidget::reload_slot(){
	FileToTabWidget::mode fttwMode;
	fttwMode = fttw_->getMode();

	mL_->removeWidget(fttw_);
	disconnect(this, SIGNAL(savedata(const string&)), fttw_, SLOT(save_file(const string&)));
	delete fttw_;

	fttw_= new FileToTabWidget(loadname_, this, fttwMode);
	mL_->insertWidget(0, fttw_);

	connect(this, SIGNAL(savedata(const string&)), fttw_, SLOT(save_file(const string&)));

	hideSomeButtons();
}

void fttw::OutSideWidget::toTextEditor(){
	emit savedata(savename_);

	mL_->removeWidget(fttw_);
	delete fttw_;

	fttw_ = new FileToTabWidget(loadname_, this, FileToTabWidget::mode::TextEditor);
	mL_->insertWidget(0, fttw_);

	connect(this, SIGNAL(savedata(const std::string&)), fttw_, SLOT(save_file(const std::string&)));

	hideSomeButtons();
}

void fttw::OutSideWidget::toTabsAndTables(){
	emit savedata(savename_);

	mL_->removeWidget(fttw_);
	delete fttw_;

	fttw_ = new FileToTabWidget(loadname_, this, FileToTabWidget::mode::Tabs);
	mL_->insertWidget(0, fttw_);

	connect(this, SIGNAL(savedata(const std::string&)), fttw_, SLOT(save_file(const std::string&)));

	hideSomeButtons();
}

void fttw::OutSideWidget::init(){
	fttw_= new FileToTabWidget(loadname_, this, FileToTabWidget::mode::TextEditor);
	mL_ = new QVBoxLayout;
	setLayout(mL_);
	setFrameStyle(NoFrame);

	saveButton_ = new QPushButton(tr("&Save"));
	reloadButton_ = new QPushButton(tr("&Reload"));
	toTextEditorButton_ = new QPushButton(tr("&Text Editor"));
	toTabsAndTablesButton_ = new QPushButton(tr("Ta&bles"));

	mL_->addWidget(fttw_);
	QHBoxLayout *saveReloadLayout = new QHBoxLayout;
	saveReloadLayout->addWidget(saveButton_);
	saveReloadLayout->addWidget(reloadButton_);
	saveReloadLayout->addWidget(toTabsAndTablesButton_);
	saveReloadLayout->addWidget(toTextEditorButton_);
	mL_->addLayout(saveReloadLayout);

	hideSomeButtons();

	connect(toTextEditorButton_, SIGNAL(clicked()), this, SLOT(toTextEditor()));
	connect(toTabsAndTablesButton_, SIGNAL(clicked()), this, SLOT(toTabsAndTables()));
	connect(saveButton_, SIGNAL(clicked()), this, SLOT(savedata_slot()));
	connect(reloadButton_, SIGNAL(clicked()), this, SLOT(reload_slot()));
	connect(this, SIGNAL(savedata(const std::string&)), fttw_, SLOT(save_file(const std::string&)));
}

fttw::OutSideWidget::OutSideWidget(const std::string &name, QWidget *pwg)
: QFrame(pwg), savename_(name), loadname_(name){
	init();
}

fttw::OutSideWidget::OutSideWidget(const std::string& savename, const std::string& loadname, QWidget* pwg)
: QFrame(pwg), savename_(savename), loadname_(loadname){
	init();
}

// ----------------------- FTTWFileBufferThread

void fttw::FTTWFileBufferThread::run(){
	FileToTabWidget::createFileBuffer(filename_, s_);
	emit finishedReading();
}

fttw::FTTWFileBufferThread::FTTWFileBufferThread(const std::string& filename, std::stringstream& s)
 : filename_(filename), s_(s){}

fttw::FTTWFileBufferThread::~FTTWFileBufferThread() {}

