% General Class Information
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{academydoc}[2015/01/01 Academy Document class]

% Load Packages
\LoadClassWithOptions{report}
\RequirePackage[hmargin={1.25in,1.25in},vmargin={1.0in,1.0in}]{geometry}
\RequirePackage{graphicx} % For Images
\graphicspath{{./images/}} % Path to images
\RequirePackage{changepage} % To allow for indented margins
\RequirePackage{ragged2e} % For Justified Text
\RequirePackage{helvet} % For Helvetica San Serifs
\RequirePackage{times} % For Times Serifs
\RequirePackage{titlesec} 
\RequirePackage{tabularx}
\RequirePackage{fancyhdr}
\RequirePackage{etoolbox}
\RequirePackage{tocloft} % Use to format the TOC
\RequirePackage{tocvsec2} % Use to get rid of the terms from the TOC
\RequirePackage[titletoc,title]{appendix}
\RequirePackage{chngcntr}
\RequirePackage{hyperref}
\RequirePackage{mathtools}
\RequirePackage{float}
\RequirePackage{ifthen}
\RequirePackage[format=plain,font=bf,labelsep=endash]{caption}
\RequirePackage[hang,flushmargin]{footmisc} 
\RequirePackage{enumitem}
\RequirePackage{amsfonts}
\RequirePackage{tabto}
\RequirePackage{listings}
\RequirePackage{pdfpages}
\RequirePackage{tikz}
\RequirePackage{textcomp}
\RequirePackage[activate=false]{microtype} % Package is off but in invoked in the AMPAS logo to letter space text

%\usepackage{showframe}

% Document Formatting
%\defaultfontfeatures{Ligatures=TeX}
\pagenumbering{arabic}
\renewcommand{\familydefault}{\rmdefault} % Set default roman font to times

\setlength{\parskip}{6pt}
\setlength{\parindent}{0pt}
\renewcommand{\baselinestretch}{1.5}
\linespread{1.0}

\makeatletter
\patchcmd{\chapter}{\if@openright\cleardoublepage\else\clearpage\fi}{\par\vspace{0.3in}}{}{} % Removes page break from chapter format
\makeatother

\def\chapterautorefname{Section}
\def\sectionautorefname{Section}
\def\subsectionautorefname{Section}

\newcommand\prelimsectionformat
{
    \setcounter{secnumdepth}{-1}
    \titleformat{\chapter}{\sffamily\fontsize{10}{12}\bfseries}{\thechapter}{0.2in}{}
    \titlespacing*{\chapter}{0pt}{-20pt}{0pt}
}

\newcommand\termsanddefsectionformat
{
    \titleformat{\section}{\fontfamily{ptm}}{\thesection}{0.2in}{}
    \titlespacing*{\section}{0pt}{0pt}{0pt}

    \settocdepth{chapter}
}
    
\newcommand\regularsectionformat
{
    \titleformat{\chapter}{\sffamily\fontsize{16}{19.2}\bfseries}{\thechapter}{0.2in}{}
    \titlespacing*{\chapter}{0pt}{-20pt}{0pt} % No indentation after the title
    \titleformat{\section}{\sffamily\fontsize{14}{16.8}\bfseries}{\thesection}{0.2in}{}
    \titlespacing*{\section}{0pt}{6pt}{0pt} % No indentation after the title
    \titleformat{\subsection}{\sffamily\fontsize{10}{12}\bfseries}{\thesubsection}{0.2in}{}
    \titlespacing*{\subsection}{0pt}{6pt}{0pt} % No indentation after the title
    \titleformat{\subsubsection}{\sffamily\fontsize{10}{12}\bfseries}{\thesubsubsection}{0.2in}{}
    \titlespacing*{\subsubsection}{0pt}{6pt}{0pt} % No indentation after the title

    \settocdepth{subsection}
}

\newcommand{\appendixsectionformat}[1]
{    
    \ifthenelse{\equal{#1}{n}}{
        \titleformat{\chapter}[display]
        {\sffamily\fontsize{14}{20}\bfseries}
        {\centering Appendix \Alph{chapter}\\*\vspace{6pt}\fontsize{10pt}{12pt}\mdseries\selectfont(normative)}
        {0in}
        {\vspace{2pt}\centering}        
    }{
        \titleformat{\chapter}[display]
        {\sffamily\fontsize{14}{20}\bfseries}
        {\centering Appendix \Alph{chapter}\\*\vspace{6pt}\fontsize{10pt}{12pt}\mdseries\selectfont(informative)}
        {0in}
        {\vspace{2pt}\centering}        
    }
    \titlespacing*{\chapter}{0pt}{-20pt}{0pt} % No indentation after the title
}

% Usage is \appendix{nameOfSection}{i/n(informative/normative)}
\newcommand{\appendixchapter}[2]
{
	\cleardoublepage

    \settocdepth{section}
    \appendixsectionformat{#2}
    \chapter{#1}
    \vspace{8pt}
}

% Table of Contents Formatting
\renewcommand*\contentsname{Table of Contents}

\renewcommand{\cfttoctitlefont}{\sffamily\fontsize{16}{19.2}\bfseries} % Table of Contents Title Font
\renewcommand{\cftchapfont}{\fontfamily{ptm}} % Chapter Font
\renewcommand{\cftchappagefont}{\fontfamily{ptm}} % Page Number Font

\cftsetindents{chapter}{0em}{1.5em}  % Indenting
\cftsetindents{section}{1.5em}{2.3em}
\cftsetindents{subsection}{3em}{3.2em}

\setlength{\cftbeforetoctitleskip}{0pt} % Vertical spacing
\setlength{\cftaftertoctitleskip}{0pt}
\setlength{\cftbeforechapskip}{6pt}
\setlength{\cftbeforesecskip}{6pt}
\setlength{\cftbeforesubsecskip}{6pt}

\renewcommand{\cftchapleader}{\cftdotfill{\cftdotsep}} % Add dot leader
\cftsetpnumwidth{12pt} % Adjust margin btween leader and page number

% Headers and footers
\fancypagestyle{plain}{%
  \fancyhf{} % clear all six fields
  \fancyhead[L]{\Docnumber}
  \fancyhead[R]{\Altdocname}
  \fancyfoot[L]{Page \thepage}
  \fancyfoot[R]{\Docdate}
  \renewcommand{\headrulewidth}{0pt}
  \renewcommand{\footrulewidth}{0pt}
}

% Table formatting
\renewcommand{\arraystretch}{1.5}    
\counterwithout{table}{chapter}

% Figure formatting
\counterwithout{figure}{chapter}

% Equation formatting
\newfloat{floatequ}{H}{loe}
\floatname{floatequ}{Equation}
\captionsetup[floatequ]{font=bf,labelfont=bf,labelsep=endash}

\makeatletter
\renewcommand\tagform@[1]{\relax}
\makeatother

    
\floatname{floatequ}{Equation}
\captionsetup[floatequ]{font=bf,labelfont=bf,labelsep=endash}

\makeatletter
\renewcommand\tagform@[1]{\relax}
\makeatother

% Code snippet formatting (e.g. CTL)
\lstset{
  tabsize=2,
  columns=fixed,
  basicstyle={\ttfamily\fontsize{8}{9.6}\selectfont}
}

% Define custom metadata commands
\newcommand{\doctype}[1]{\newcommand\Doctype{#1}}
\newcommand{\docname}[1]{\newcommand\Docname{#1}}
\newcommand{\altdocname}[1]{\newcommand\Altdocname{#1}}
\newcommand{\docnumber}[1]{\newcommand\Docnumber{#1}}
\newcommand{\committeename}[1]{\newcommand\Committeename{#1}}
\newcommand{\versionnumber}[1]{\newcommand\Versionnumber{#1}}
\newcommand{\docdate}[1]{\newcommand\Docdate{#1}}
\newcommand{\summary}[1]{\newcommand\Summary{#1}}
\newcommand{\noticestext}[1]{\newcommand\Noticestext{#1}}
\newcommand{\revisiontable}[1]{\newcommand\Revisiontable{#1}}
\newcommand{\ampasdocstable}[1]{\newcommand\Ampasdocstable{#1}}
\newcommand{\acknowledgementstable}[1]{\newcommand\Acknowledgementstable{#1}}
\newcommand{\introductiontext}[1]{\newcommand\Introductiontext{#1}}
\newcommand{\listoftermsanddefs}[1]{\newcommand\Listoftermsanddefs{#1}}

% Set format for bulleted lists
\let\olditemize\itemize
\renewcommand{\itemize}{
    \vspace{-6pt}
    \olditemize[leftmargin=0.25in]
    \setlength{\itemsep}{6pt}
    \setlength{\parskip}{0pt}
    \setlength{\parsep}{0pt}
    \setlength{\topsep}{0pt}
}

\newenvironment{listize}[1][]{
    \begin{list}{#1}{
        \setlength{\itemsep}{6pt}
        \setlength{\parskip}{0pt}
        \setlength{\parsep}{0pt}
        \setlength{\topsep}{6pt}
    }
}{
    \end{list}
}

% Set format for enumerated lists
\let\oldenumerate\enumerate
\renewcommand{\enumerate}{
    \vspace{-6pt}
    \oldenumerate[leftmargin=0.25in]
    \setlength{\itemsep}{6pt}
    \setlength{\parskip}{0pt}
    \setlength{\parsep}{0pt}
    \setlength{\topsep}{0pt}
}

% Redefine \maketitle for creating the title page
\renewcommand{\maketitle}
{
    % Insert Academy Logo
    \thispagestyle{empty}
    \begin{figure}[t]
	    \vspace*{0.5in}
	    \centering
	    \begin{tikzpicture}[y=0.80pt, x=0.80pt, yscale=-0.3, xscale=0.3, inner sep=0pt, outer sep=0pt]
	    	\begin{scope}
	    	\path[fill=black] (0.1000,258.8000) -- (114.0000,0.2000) -- (227.9000,258.7000) --
		      (155.6000,258.7000) -- (155.6000,258.7000) -- (146.7000,237.6000) .. controls
		      (146.3000,236.7000) and (145.9000,236.5000) .. (145.0000,236.5000) --
		      (135.8000,236.5000) .. controls (135.0000,236.5000) and (134.5000,236.3000) ..
		      (134.2000,235.6000) .. controls (133.0000,233.0000) and (130.9000,232.0000) ..
		      (128.1000,232.0000) .. controls (126.6000,232.0000) and (125.0000,231.8000) ..
		      (123.5000,231.5000) .. controls (120.6000,230.9000) and (119.5000,229.5000) ..
		      (119.3000,226.6000) .. controls (119.0000,222.3000) and (119.7000,218.2000) ..
		      (120.4000,214.0000) .. controls (121.2000,209.0000) and (122.4000,204.0000) ..
		      (122.0000,198.9000) .. controls (121.9000,198.0000) and (121.9000,197.1000) ..
		      (121.8000,196.3000) .. controls (121.3000,193.1000) and (121.8000,190.0000) ..
		      (122.7000,186.8000) .. controls (125.2000,178.2000) and (125.3000,169.5000) ..
		      (123.0000,160.8000) .. controls (123.1000,160.7000) and (123.1000,160.6000) ..
		      (123.1000,160.6000) .. controls (126.1000,159.9000) and (127.9000,158.1000) ..
		      (128.5000,155.0000) .. controls (129.5000,149.9000) and (130.6000,144.8000) ..
		      (131.4000,139.7000) .. controls (131.7000,138.2000) and (131.4000,136.6000) ..
		      (131.2000,135.1000) .. controls (130.9000,132.7000) and (129.5000,131.1000) ..
		      (127.4000,130.1000) .. controls (126.0000,129.5000) and (124.5000,129.1000) ..
		      (123.2000,128.4000) .. controls (121.9000,127.7000) and (120.7000,127.0000) ..
		      (119.7000,126.0000) .. controls (118.4000,124.9000) and (118.3000,122.7000) ..
		      (119.1000,121.1000) .. controls (119.5000,120.3000) and (119.9000,119.4000) ..
		      (120.0000,118.5000) .. controls (120.2000,116.4000) and (120.5000,114.3000) ..
		      (120.4000,112.2000) .. controls (120.2000,108.8000) and (117.3000,106.6000) ..
		      (113.6000,106.7000) .. controls (110.4000,106.8000) and (107.9000,109.4000) ..
		      (107.9000,112.6000) .. controls (107.9000,115.7000) and (107.9000,118.8000) ..
		      (109.5000,121.7000) .. controls (110.3000,123.2000) and (109.3000,125.7000) ..
		      (107.5000,126.9000) .. controls (106.6000,127.6000) and (105.5000,128.0000) ..
		      (104.4000,128.5000) .. controls (103.2000,129.0000) and (101.9000,129.4000) ..
		      (100.7000,130.0000) .. controls (98.6000,131.0000) and (97.3000,132.6000) ..
		      (96.9000,135.0000) .. controls (96.4000,138.1000) and (96.7000,141.2000) ..
		      (97.3000,144.3000) .. controls (98.1000,148.1000) and (99.0000,151.8000) ..
		      (99.8000,155.5000) .. controls (100.4000,158.2000) and (102.1000,159.7000) ..
		      (104.6000,160.4000) .. controls (104.7000,160.4000) and (104.8000,160.5000) ..
		      (104.9000,160.5000) .. controls (104.9000,160.5000) and (105.0000,160.6000) ..
		      (104.8000,160.4000) .. controls (104.3000,163.9000) and (103.8000,167.2000) ..
		      (103.5000,170.5000) .. controls (103.0000,176.0000) and (103.8000,181.4000) ..
		      (105.3000,186.7000) .. controls (106.2000,189.9000) and (106.6000,193.0000) ..
		      (106.2000,196.3000) .. controls (105.5000,201.3000) and (106.2000,206.2000) ..
		      (107.0000,211.1000) .. controls (107.7000,215.2000) and (108.2000,219.4000) ..
		      (108.6000,223.6000) .. controls (108.7000,225.0000) and (108.5000,226.5000) ..
		      (108.4000,227.9000) .. controls (108.2000,229.7000) and (107.1000,230.9000) ..
		      (105.4000,231.3000) .. controls (103.5000,231.7000) and (101.5000,232.0000) ..
		      (99.6000,232.1000) .. controls (96.9000,232.2000) and (94.9000,233.1000) ..
		      (93.7000,235.6000) .. controls (93.3000,236.4000) and (92.7000,236.6000) ..
		      (91.9000,236.6000) -- (82.7000,236.6000) .. controls (81.9000,236.6000) and
		      (81.5000,236.8000) .. (81.2000,237.6000) -- (72.7000,258.7000) --
		      (0.1000,258.8000) -- cycle;
	    	\end{scope}
	  		\path[cm={{1.0,0.0,0.0,1.0,(112,258)}}] (0.0000,0.0000) node[above] () {\fontsize{2}{2} \sffamily\selectfont{\textls[250] {\textcopyright AMPAS\textregistered}}};
	    \end{tikzpicture}
    	\centering
	    \vspace{0.5in}
    \end{figure}

	\begin{center}
	    \fontsize{18}{21.6} 
		\selectfont

        \ifthenelse{\equal{\Doctype}{spec}}{
            \sffamily{\textbf{Specification}} \\
        }{
            \ifthenelse{\equal{\Doctype}{tb}}{
                \sffamily{\textbf{Technical Bulletin}} \\
            }{
                \ifthenelse{\equal{\Doctype}{proc}}{
                    \sffamily{\textbf{Procedure}} \\
                }{
                    \sffamily{\textbf{}} \\
                }
            }
        }
		\vspace{12pt}
		\sffamily{\textbf{\Docnumber}} \\ 
		\vspace{12pt}
        \sffamily{\textbf{\Docname}} \\

		\vspace{22pt}
		\fontsize{14}{16.8} 
		\selectfont

		\rmfamily{The Academy of Motion Picture Arts and Sciences} \\
		\vspace{6pt}
		Science and Technology Council \\
		\vspace{6pt}
		\Committeename \\

		\vspace{22pt}
		\fontsize{11}{13.2} 
		\selectfont
		\Docdate \\
		\vspace{18pt}
	\end{center}
	\begin{adjustwidth}{0.5in}{0.5in}
        \begin{justify}
            \fontsize{11}{13.2} 
            \selectfont
            \setlength{\parindent}{0pt}
            Summary: \Summary
        \end{justify}
	\end{adjustwidth}
	\cleardoublepage
}

\newcommand{\unnumberedformat}
{
    \regularsectionformat
    \setcounter{secnumdepth}{-1}
}

\newcommand{\numberedformat}
{
    \regularsectionformat
    \setcounter{secnumdepth}{9}
}

\newcommand{\term}[1]
{
    \termsanddefsectionformat
    
    \section{#1}
    
    \regularsectionformat
}

% Make a note macro that autoresets with subsection or above 
\newcounter{note}

\makeatletter
\@addtoreset{note}{chapter}
\@addtoreset{note}{section}
\@addtoreset{note}{subsection}
\@addtoreset{note}{subsubsection}
\makeatother


\newcommand{\note}[1]
{
    \ifthenelse{ \arabic{note} > 0}{
	    NOTE \arabic{note}: #1
	}{
	    NOTE: #1
	    \stepcounter{note}
	}
	\stepcounter{note}
}

% Formatting for XML elements
\def\xmlitem[#1][#2]#3{\item[{\texttt{#1}}] \tabto{0.5\textwidth}(#2) \\ #3}
\def\xmlitemopt[#1][#2]{\item[{\texttt{#1}}]  \tabto{0.5\textwidth}(#2)}
\def\xmlitemd[#1]#2{\item[{\texttt{#1}}] \hfill \\ #2}

\newenvironment{xmlfields}{
	\renewcommand*\descriptionlabel[1]{\hspace\labelsep\texttt{##1}}
	\begin{description}[itemindent=-0.2in,leftmargin=0.5in,itemsep=6pt,topsep=0pt]
}{	
	\end{description}
	\vspace{6pt}
}	
