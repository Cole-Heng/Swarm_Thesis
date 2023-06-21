\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{thesis}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% OPTIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\DeclareOption{singlespacing}{\def\myspacing{\singlespacing}}
\DeclareOption{onehalfspacing}{\def\myspacing{\onehalfspacing}}

\ExecuteOptions{onehalfspacing}  % default options

\ProcessOptions\relax

\LoadClass[12pt,oneside]{book}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% PACKAGES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage[T1]{fontenc}             % font encoding
\RequirePackage[utf8]{inputenc}			   	 % handle accents properly
\RequirePackage[english]{babel}          % handle hyphenation properly
\RequirePackage{lmodern}                 % computer modern font
\RequirePackage[margin=1in]{geometry}    % formatting
\RequirePackage[titles]{tocloft}         % table of contents and list of figures
\RequirePackage{titlesec}                % title section
\RequirePackage{hyperref}                % links
\RequirePackage{setspace}                % spacing
\RequirePackage{fancyhdr}                % fancy header/footer
\RequirePackage{etoolbox}                % edit latex macros

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% SECTIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\titleformat{\chapter}{\filcenter}{}{0pt}{\Large\bfseries\sffamily}
\titleformat{\section}{\bfseries\sffamily}{\thesection}{1em}{}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CHAPTER HEADINGS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@makechapterhead#1{\vbox to 15pc{%
  \vspace*{15\p@}%
  {\parindent \z@ \raggedright \normalfont
    \if@mainmatter
      {\normalfont\sffamily\Huge\bfseries\centering\thechapter\par}
    \else
      \vphantom{\normalfont\sffamily\Huge\bfseries\centering\thechapter}\par
    \fi
    \addvspace{10pt}\nobreak
    \noindent\hrule\@width1\columnwidth\@height1\p@
    \vskip 14\p@
    \interlinepenalty\@M
    {\normalfont\sffamily\LARGE\bfseries\centering\boldmath#1\strut\par}\addvspace{10pt}\nobreak
    \noindent\hrule\@width1\columnwidth\@height1\p@
    \vfill
  }}}
\def\@schapter#1{\@makechapterhead{#1}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% TOC / LOT / LOF
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\renewcommand*\contentsname{\centering Table of Contents}
\renewcommand{\cfttoctitlefont}{\hspace*{\fill}\Large\bfseries\sffamily}
\renewcommand{\cftaftertoctitle}{\hspace*{\fill}}
\renewcommand*{\cftlottitlefont}{\hspace*{\fill}\Large\bfseries\sffamily}
\renewcommand{\cftafterlottitle}{\hspace*{\fill}}
\renewcommand{\cftloftitlefont}{\hspace*{\fill}\Large\bfseries\sffamily}
\renewcommand{\cftafterloftitle}{\hspace*{\fill}}

\setcounter{tocdepth}{2}

\def\lot{\addcontentsline{toc}{chapter}{\listtablename}\listoftables}
\def\lof{\addcontentsline{toc}{chapter}{\listfigurename}\listoffigures}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% HEADER / FOOTER
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\pagestyle{fancy}        % set page style to "fancy"
\fancyhf{}               % clear all headers and footers
\fancyhead{}             % header (empty)
\fancyfoot[C]{\thepage}  % footer (page number centered)
\renewcommand{\headrulewidth}{0pt}  % remove headrule

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% COMMANDS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@firstname{}
\def\@lastname{}
\def\@year{}
\def\@school{}
\def\@college{}
\def\@department{}
\def\@city{}
\def\@state{}
\def\@advisor1{}
\def\@advisor2{}

\newcommand*{\name}[2]{\def\@firstname{#1}\def\@lastname{#2}}
\renewcommand*{\year}[1]{\def\@year{#1}}
\newcommand*{\school}[1]{\def\@school{#1}}
\newcommand*{\college}[1]{\def\@college{#1}}
\newcommand*{\department}[1]{\def\@department{#1}}
\newcommand*{\location}[2]{\def\@city{#1}\def\@state{#2}}
\newcommand*{\advisor}[1]{\def\@advisor{#1}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% METADATA
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\hypersetup{
  pdfauthor  = {\@firstname~\@lastname},
  pdftitle   = {\@title},
  pdfsubject = {},
  colorlinks,
  allcolors  = black,
  linktoc    = all,
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% ABSTRACT (max 200 words)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newenvironment{abstract}
  {\thispagestyle{empty}%
  \singlespacing%
  \begin{center}
    {ABSTRACT}\\[12pt]
    \MakeUppercase{\@title}\\[12pt]
    by \@firstname~\@lastname\\[24pt]
  \end{center}
  \parindent0pt}
  {\newpage}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% TITLE
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\renewcommand*{\maketitle}{%
  \thispagestyle{empty}%
  \begin{titlepage}
    \centering
    \begin{singlespacing}
      \MakeUppercase{\@title}
    \end{singlespacing}
    \vfill
    {\openup 1em
      A Thesis\\[1cm]
      Submitted to the\\
      Faculty of \@school\\
      in partial fulfillment of\\
      the requirements for the degree of\\
      Master of Science\\
      by\\
      \@firstname~\@lastname\\
      \@school\\
      \@city,~\@state\\
      \@year\\
      \vfill
      \renewcommand{\do}[1]{Advisor: ##1\\}{\dolistloop\advisors}
      \renewcommand{\do}[1]{Reader: ##1\\}{\dolistloop\committee}
    }
    \vfill
    \textcopyright~\@year~\@firstname~\@lastname
    \vfill
    \newpage
    \thispagestyle{empty}
    This thesis titled\\[1cm]
    \MakeUppercase{\@title}\\[1cm]
    by\\[1cm]
    \@firstname~\@lastname\\[1cm]
    has been approved for publication by\\[0.75cm]
    The College of~\@college\\[0.5cm]
    and\\[0.5cm]
    The Department of~\@department\\[1.5cm]
    % \rule[-0.1cm]{8cm}{0.01cm}\\
    % Dr. Advisor\\[1cm]
    \renewcommand{\do}[1]{\rule[-0.1cm]{8cm}{0.01cm}\\##1\\[1cm]}{\dolistloop\advisors}
    \renewcommand{\do}[1]{\rule[-0.1cm]{8cm}{0.01cm}\\##1\\[1cm]}{\dolistloop\committee}
  \end{titlepage}
  \myspacing}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% BIBLIOGRAPHY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\bibliographystyle{unsrt}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% APPENDICES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\appendices{\appendix%
  \setcounter{chapter}{0}%
  \renewcommand{\thechapter}{\Alph{chapter}}}