// Generated from MiniC.g4 by ANTLR 4.13.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class MiniCParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, T__27=28, T__28=29, T__29=30, T__30=31, 
		T__31=32, T__32=33, T__33=34, ID=35, INT=36, CHAR=37, WS=38, COMMENT=39;
	public static final int
		RULE_prog = 0, RULE_preamble = 1, RULE_decl = 2, RULE_vardecl = 3, RULE_scope = 4, 
		RULE_stmt_seq = 5, RULE_stmt = 6, RULE_varlist = 7, RULE_varlistentry = 8, 
		RULE_vartype = 9, RULE_rettype = 10, RULE_parameters = 11, RULE_parameterlist = 12, 
		RULE_parameterentry = 13, RULE_expropt = 14, RULE_expr = 15, RULE_var = 16, 
		RULE_arguments = 17, RULE_argumentlist = 18, RULE_varname = 19, RULE_funcname = 20, 
		RULE_parametername = 21;
	private static String[] makeRuleNames() {
		return new String[] {
			"prog", "preamble", "decl", "vardecl", "scope", "stmt_seq", "stmt", "varlist", 
			"varlistentry", "vartype", "rettype", "parameters", "parameterlist", 
			"parameterentry", "expropt", "expr", "var", "arguments", "argumentlist", 
			"varname", "funcname", "parametername"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'#include'", "'\"minicio.h\"'", "'('", "')'", "';'", "'{'", "'}'", 
			"'if'", "'else'", "'for'", "'while'", "'break'", "'return'", "'['", "']'", 
			"','", "'int'", "'bool'", "'char'", "'void'", "'-'", "'!'", "'*'", "'/'", 
			"'+'", "'true'", "'false'", "'=='", "'!='", "'<'", "'='", "'>'", "'&&'", 
			"'||'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, "ID", 
			"INT", "CHAR", "WS", "COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "MiniC.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public MiniCParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ProgContext extends ParserRuleContext {
		public PreambleContext preamble() {
			return getRuleContext(PreambleContext.class,0);
		}
		public DeclContext decl() {
			return getRuleContext(DeclContext.class,0);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterProg(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitProg(this);
		}
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_prog);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(44);
			preamble();
			setState(45);
			decl(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PreambleContext extends ParserRuleContext {
		public PreambleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_preamble; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterPreamble(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitPreamble(this);
		}
	}

	public final PreambleContext preamble() throws RecognitionException {
		PreambleContext _localctx = new PreambleContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_preamble);
		try {
			setState(50);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
				enterOuterAlt(_localctx, 1);
				{
				setState(47);
				match(T__0);
				setState(48);
				match(T__1);
				}
				break;
			case T__16:
			case T__17:
			case T__18:
			case T__19:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DeclContext extends ParserRuleContext {
		public VardeclContext vardecl() {
			return getRuleContext(VardeclContext.class,0);
		}
		public RettypeContext rettype() {
			return getRuleContext(RettypeContext.class,0);
		}
		public FuncnameContext funcname() {
			return getRuleContext(FuncnameContext.class,0);
		}
		public ParametersContext parameters() {
			return getRuleContext(ParametersContext.class,0);
		}
		public ScopeContext scope() {
			return getRuleContext(ScopeContext.class,0);
		}
		public List<DeclContext> decl() {
			return getRuleContexts(DeclContext.class);
		}
		public DeclContext decl(int i) {
			return getRuleContext(DeclContext.class,i);
		}
		public DeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_decl; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterDecl(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitDecl(this);
		}
	}

	public final DeclContext decl() throws RecognitionException {
		return decl(0);
	}

	private DeclContext decl(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		DeclContext _localctx = new DeclContext(_ctx, _parentState);
		DeclContext _prevctx = _localctx;
		int _startState = 4;
		enterRecursionRule(_localctx, 4, RULE_decl, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(68);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,1,_ctx) ) {
			case 1:
				{
				setState(53);
				vardecl(0);
				}
				break;
			case 2:
				{
				setState(54);
				rettype();
				setState(55);
				funcname();
				setState(56);
				match(T__2);
				setState(57);
				parameters();
				setState(58);
				match(T__3);
				setState(59);
				scope();
				}
				break;
			case 3:
				{
				setState(61);
				rettype();
				setState(62);
				funcname();
				setState(63);
				match(T__2);
				setState(64);
				parameters();
				setState(65);
				match(T__3);
				setState(66);
				match(T__4);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(74);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,2,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new DeclContext(_parentctx, _parentState);
					pushNewRecursionContext(_localctx, _startState, RULE_decl);
					setState(70);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(71);
					decl(2);
					}
					} 
				}
				setState(76);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,2,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VardeclContext extends ParserRuleContext {
		public VartypeContext vartype() {
			return getRuleContext(VartypeContext.class,0);
		}
		public VarlistContext varlist() {
			return getRuleContext(VarlistContext.class,0);
		}
		public List<VardeclContext> vardecl() {
			return getRuleContexts(VardeclContext.class);
		}
		public VardeclContext vardecl(int i) {
			return getRuleContext(VardeclContext.class,i);
		}
		public VardeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_vardecl; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterVardecl(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitVardecl(this);
		}
	}

	public final VardeclContext vardecl() throws RecognitionException {
		return vardecl(0);
	}

	private VardeclContext vardecl(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		VardeclContext _localctx = new VardeclContext(_ctx, _parentState);
		VardeclContext _prevctx = _localctx;
		int _startState = 6;
		enterRecursionRule(_localctx, 6, RULE_vardecl, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(78);
			vartype();
			setState(79);
			varlist(0);
			setState(80);
			match(T__4);
			}
			_ctx.stop = _input.LT(-1);
			setState(86);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,3,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new VardeclContext(_parentctx, _parentState);
					pushNewRecursionContext(_localctx, _startState, RULE_vardecl);
					setState(82);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(83);
					vardecl(2);
					}
					} 
				}
				setState(88);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,3,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ScopeContext extends ParserRuleContext {
		public VardeclContext vardecl() {
			return getRuleContext(VardeclContext.class,0);
		}
		public Stmt_seqContext stmt_seq() {
			return getRuleContext(Stmt_seqContext.class,0);
		}
		public ScopeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_scope; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterScope(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitScope(this);
		}
	}

	public final ScopeContext scope() throws RecognitionException {
		ScopeContext _localctx = new ScopeContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_scope);
		try {
			setState(100);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(89);
				match(T__5);
				setState(90);
				vardecl(0);
				setState(91);
				stmt_seq();
				setState(92);
				match(T__6);
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(94);
				match(T__5);
				setState(95);
				stmt_seq();
				setState(96);
				match(T__6);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(98);
				match(T__5);
				setState(99);
				match(T__6);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Stmt_seqContext extends ParserRuleContext {
		public List<StmtContext> stmt() {
			return getRuleContexts(StmtContext.class);
		}
		public StmtContext stmt(int i) {
			return getRuleContext(StmtContext.class,i);
		}
		public Stmt_seqContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stmt_seq; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterStmt_seq(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitStmt_seq(this);
		}
	}

	public final Stmt_seqContext stmt_seq() throws RecognitionException {
		Stmt_seqContext _localctx = new Stmt_seqContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_stmt_seq);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(103); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(102);
					stmt();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(105); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,5,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StmtContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public Stmt_seqContext stmt_seq() {
			return getRuleContext(Stmt_seqContext.class,0);
		}
		public List<ExproptContext> expropt() {
			return getRuleContexts(ExproptContext.class);
		}
		public ExproptContext expropt(int i) {
			return getRuleContext(ExproptContext.class,i);
		}
		public ScopeContext scope() {
			return getRuleContext(ScopeContext.class,0);
		}
		public StmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stmt; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterStmt(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitStmt(this);
		}
	}

	public final StmtContext stmt() throws RecognitionException {
		StmtContext _localctx = new StmtContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_stmt);
		try {
			setState(147);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,6,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(107);
				expr(0);
				setState(108);
				match(T__4);
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(110);
				match(T__7);
				setState(111);
				match(T__2);
				setState(112);
				expr(0);
				setState(113);
				match(T__3);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(115);
				match(T__7);
				setState(116);
				match(T__2);
				setState(117);
				expr(0);
				setState(118);
				match(T__3);
				setState(119);
				match(T__8);
				setState(120);
				stmt_seq();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(122);
				match(T__9);
				setState(123);
				match(T__2);
				setState(124);
				expropt();
				setState(125);
				match(T__4);
				setState(126);
				expropt();
				setState(127);
				match(T__4);
				setState(128);
				expropt();
				setState(129);
				match(T__3);
				setState(130);
				stmt_seq();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(132);
				match(T__10);
				setState(133);
				match(T__2);
				setState(134);
				expr(0);
				setState(135);
				match(T__3);
				setState(136);
				stmt_seq();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(138);
				match(T__11);
				setState(139);
				match(T__4);
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(140);
				match(T__12);
				setState(141);
				match(T__4);
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(142);
				match(T__12);
				setState(143);
				expr(0);
				setState(144);
				match(T__4);
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(146);
				scope();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VarlistContext extends ParserRuleContext {
		public VarnameContext varname() {
			return getRuleContext(VarnameContext.class,0);
		}
		public TerminalNode INT() { return getToken(MiniCParser.INT, 0); }
		public List<VarlistContext> varlist() {
			return getRuleContexts(VarlistContext.class);
		}
		public VarlistContext varlist(int i) {
			return getRuleContext(VarlistContext.class,i);
		}
		public VarlistContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varlist; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterVarlist(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitVarlist(this);
		}
	}

	public final VarlistContext varlist() throws RecognitionException {
		return varlist(0);
	}

	private VarlistContext varlist(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		VarlistContext _localctx = new VarlistContext(_ctx, _parentState);
		VarlistContext _prevctx = _localctx;
		int _startState = 14;
		enterRecursionRule(_localctx, 14, RULE_varlist, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(156);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
			case 1:
				{
				setState(150);
				varname();
				}
				break;
			case 2:
				{
				setState(151);
				varname();
				setState(152);
				match(T__13);
				setState(153);
				match(INT);
				setState(154);
				match(T__14);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(163);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new VarlistContext(_parentctx, _parentState);
					pushNewRecursionContext(_localctx, _startState, RULE_varlist);
					setState(158);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(159);
					match(T__15);
					setState(160);
					varlist(2);
					}
					} 
				}
				setState(165);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VarlistentryContext extends ParserRuleContext {
		public VarlistentryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varlistentry; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterVarlistentry(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitVarlistentry(this);
		}
	}

	public final VarlistentryContext varlistentry() throws RecognitionException {
		VarlistentryContext _localctx = new VarlistentryContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_varlistentry);
		try {
			enterOuterAlt(_localctx, 1);
			{
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VartypeContext extends ParserRuleContext {
		public VartypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_vartype; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterVartype(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitVartype(this);
		}
	}

	public final VartypeContext vartype() throws RecognitionException {
		VartypeContext _localctx = new VartypeContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_vartype);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(168);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 917504L) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class RettypeContext extends ParserRuleContext {
		public VartypeContext vartype() {
			return getRuleContext(VartypeContext.class,0);
		}
		public RettypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_rettype; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterRettype(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitRettype(this);
		}
	}

	public final RettypeContext rettype() throws RecognitionException {
		RettypeContext _localctx = new RettypeContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_rettype);
		try {
			setState(172);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__19:
				enterOuterAlt(_localctx, 1);
				{
				setState(170);
				match(T__19);
				}
				break;
			case T__16:
			case T__17:
			case T__18:
				enterOuterAlt(_localctx, 2);
				{
				setState(171);
				vartype();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParametersContext extends ParserRuleContext {
		public ParameterlistContext parameterlist() {
			return getRuleContext(ParameterlistContext.class,0);
		}
		public ParametersContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parameters; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterParameters(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitParameters(this);
		}
	}

	public final ParametersContext parameters() throws RecognitionException {
		ParametersContext _localctx = new ParametersContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_parameters);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(175);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 917504L) != 0)) {
				{
				setState(174);
				parameterlist(0);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParameterlistContext extends ParserRuleContext {
		public VartypeContext vartype() {
			return getRuleContext(VartypeContext.class,0);
		}
		public ParameternameContext parametername() {
			return getRuleContext(ParameternameContext.class,0);
		}
		public List<ParameterlistContext> parameterlist() {
			return getRuleContexts(ParameterlistContext.class);
		}
		public ParameterlistContext parameterlist(int i) {
			return getRuleContext(ParameterlistContext.class,i);
		}
		public ParameterlistContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parameterlist; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterParameterlist(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitParameterlist(this);
		}
	}

	public final ParameterlistContext parameterlist() throws RecognitionException {
		return parameterlist(0);
	}

	private ParameterlistContext parameterlist(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ParameterlistContext _localctx = new ParameterlistContext(_ctx, _parentState);
		ParameterlistContext _prevctx = _localctx;
		int _startState = 24;
		enterRecursionRule(_localctx, 24, RULE_parameterlist, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(178);
			vartype();
			setState(179);
			parametername();
			}
			_ctx.stop = _input.LT(-1);
			setState(186);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,11,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new ParameterlistContext(_parentctx, _parentState);
					pushNewRecursionContext(_localctx, _startState, RULE_parameterlist);
					setState(181);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(182);
					match(T__15);
					setState(183);
					parameterlist(2);
					}
					} 
				}
				setState(188);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,11,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParameterentryContext extends ParserRuleContext {
		public ParameterentryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parameterentry; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterParameterentry(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitParameterentry(this);
		}
	}

	public final ParameterentryContext parameterentry() throws RecognitionException {
		ParameterentryContext _localctx = new ParameterentryContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_parameterentry);
		try {
			enterOuterAlt(_localctx, 1);
			{
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExproptContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public ExproptContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expropt; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterExpropt(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitExpropt(this);
		}
	}

	public final ExproptContext expropt() throws RecognitionException {
		ExproptContext _localctx = new ExproptContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_expropt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(192);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 240725786632L) != 0)) {
				{
				setState(191);
				expr(0);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExprContext extends ParserRuleContext {
		public TerminalNode INT() { return getToken(MiniCParser.INT, 0); }
		public TerminalNode CHAR() { return getToken(MiniCParser.CHAR, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public VarContext var() {
			return getRuleContext(VarContext.class,0);
		}
		public FuncnameContext funcname() {
			return getRuleContext(FuncnameContext.class,0);
		}
		public ArgumentsContext arguments() {
			return getRuleContext(ArgumentsContext.class,0);
		}
		public ParameternameContext parametername() {
			return getRuleContext(ParameternameContext.class,0);
		}
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterExpr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitExpr(this);
		}
	}

	public final ExprContext expr() throws RecognitionException {
		return expr(0);
	}

	private ExprContext expr(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExprContext _localctx = new ExprContext(_ctx, _parentState);
		ExprContext _prevctx = _localctx;
		int _startState = 30;
		enterRecursionRule(_localctx, 30, RULE_expr, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(218);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,13,_ctx) ) {
			case 1:
				{
				setState(195);
				match(INT);
				}
				break;
			case 2:
				{
				setState(196);
				match(CHAR);
				}
				break;
			case 3:
				{
				setState(197);
				match(T__20);
				setState(198);
				expr(14);
				}
				break;
			case 4:
				{
				setState(199);
				match(T__21);
				setState(200);
				expr(13);
				}
				break;
			case 5:
				{
				setState(201);
				match(T__25);
				}
				break;
			case 6:
				{
				setState(202);
				match(T__26);
				}
				break;
			case 7:
				{
				setState(203);
				match(T__2);
				setState(204);
				expr(0);
				setState(205);
				match(T__3);
				}
				break;
			case 8:
				{
				setState(207);
				var();
				}
				break;
			case 9:
				{
				setState(208);
				funcname();
				setState(209);
				match(T__2);
				setState(210);
				arguments();
				setState(211);
				match(T__3);
				}
				break;
			case 10:
				{
				setState(213);
				var();
				setState(214);
				match(T__30);
				setState(215);
				expr(2);
				}
				break;
			case 11:
				{
				setState(217);
				parametername();
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(246);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(244);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,15,_ctx) ) {
					case 1:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(220);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						setState(221);
						_la = _input.LA(1);
						if ( !(_la==T__22 || _la==T__23) ) {
						_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(222);
						expr(13);
						}
						break;
					case 2:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(223);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						setState(224);
						_la = _input.LA(1);
						if ( !(_la==T__20 || _la==T__24) ) {
						_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(225);
						expr(12);
						}
						break;
					case 3:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(226);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						setState(235);
						_errHandler.sync(this);
						switch ( getInterpreter().adaptivePredict(_input,14,_ctx) ) {
						case 1:
							{
							setState(227);
							match(T__27);
							}
							break;
						case 2:
							{
							setState(228);
							match(T__28);
							}
							break;
						case 3:
							{
							setState(229);
							match(T__29);
							}
							break;
						case 4:
							{
							setState(230);
							match(T__29);
							setState(231);
							match(T__30);
							}
							break;
						case 5:
							{
							setState(232);
							match(T__31);
							}
							break;
						case 6:
							{
							setState(233);
							match(T__31);
							setState(234);
							match(T__30);
							}
							break;
						}
						setState(237);
						expr(9);
						}
						break;
					case 4:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(238);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						setState(239);
						match(T__32);
						setState(240);
						expr(8);
						}
						break;
					case 5:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(241);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						setState(242);
						match(T__33);
						setState(243);
						expr(7);
						}
						break;
					}
					} 
				}
				setState(248);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VarContext extends ParserRuleContext {
		public VarnameContext varname() {
			return getRuleContext(VarnameContext.class,0);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public VarContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_var; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterVar(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitVar(this);
		}
	}

	public final VarContext var() throws RecognitionException {
		VarContext _localctx = new VarContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_var);
		try {
			setState(255);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(249);
				varname();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(250);
				varname();
				setState(251);
				match(T__13);
				setState(252);
				expr(0);
				setState(253);
				match(T__14);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ArgumentsContext extends ParserRuleContext {
		public ArgumentlistContext argumentlist() {
			return getRuleContext(ArgumentlistContext.class,0);
		}
		public ArgumentsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arguments; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterArguments(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitArguments(this);
		}
	}

	public final ArgumentsContext arguments() throws RecognitionException {
		ArgumentsContext _localctx = new ArgumentsContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_arguments);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(258);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 240725786632L) != 0)) {
				{
				setState(257);
				argumentlist(0);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ArgumentlistContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public List<ArgumentlistContext> argumentlist() {
			return getRuleContexts(ArgumentlistContext.class);
		}
		public ArgumentlistContext argumentlist(int i) {
			return getRuleContext(ArgumentlistContext.class,i);
		}
		public ArgumentlistContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_argumentlist; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterArgumentlist(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitArgumentlist(this);
		}
	}

	public final ArgumentlistContext argumentlist() throws RecognitionException {
		return argumentlist(0);
	}

	private ArgumentlistContext argumentlist(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ArgumentlistContext _localctx = new ArgumentlistContext(_ctx, _parentState);
		ArgumentlistContext _prevctx = _localctx;
		int _startState = 36;
		enterRecursionRule(_localctx, 36, RULE_argumentlist, _p);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(261);
			expr(0);
			}
			_ctx.stop = _input.LT(-1);
			setState(268);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,19,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new ArgumentlistContext(_parentctx, _parentState);
					pushNewRecursionContext(_localctx, _startState, RULE_argumentlist);
					setState(263);
					if (!(precpred(_ctx, 1))) throw new FailedPredicateException(this, "precpred(_ctx, 1)");
					setState(264);
					match(T__15);
					setState(265);
					argumentlist(2);
					}
					} 
				}
				setState(270);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,19,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VarnameContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(MiniCParser.ID, 0); }
		public VarnameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varname; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterVarname(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitVarname(this);
		}
	}

	public final VarnameContext varname() throws RecognitionException {
		VarnameContext _localctx = new VarnameContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_varname);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(271);
			match(ID);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FuncnameContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(MiniCParser.ID, 0); }
		public FuncnameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcname; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterFuncname(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitFuncname(this);
		}
	}

	public final FuncnameContext funcname() throws RecognitionException {
		FuncnameContext _localctx = new FuncnameContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_funcname);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(273);
			match(ID);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParameternameContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(MiniCParser.ID, 0); }
		public ParameternameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parametername; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).enterParametername(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof MiniCListener ) ((MiniCListener)listener).exitParametername(this);
		}
	}

	public final ParameternameContext parametername() throws RecognitionException {
		ParameternameContext _localctx = new ParameternameContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_parametername);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(275);
			match(ID);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 2:
			return decl_sempred((DeclContext)_localctx, predIndex);
		case 3:
			return vardecl_sempred((VardeclContext)_localctx, predIndex);
		case 7:
			return varlist_sempred((VarlistContext)_localctx, predIndex);
		case 12:
			return parameterlist_sempred((ParameterlistContext)_localctx, predIndex);
		case 15:
			return expr_sempred((ExprContext)_localctx, predIndex);
		case 18:
			return argumentlist_sempred((ArgumentlistContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean decl_sempred(DeclContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean vardecl_sempred(VardeclContext _localctx, int predIndex) {
		switch (predIndex) {
		case 1:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean varlist_sempred(VarlistContext _localctx, int predIndex) {
		switch (predIndex) {
		case 2:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean parameterlist_sempred(ParameterlistContext _localctx, int predIndex) {
		switch (predIndex) {
		case 3:
			return precpred(_ctx, 1);
		}
		return true;
	}
	private boolean expr_sempred(ExprContext _localctx, int predIndex) {
		switch (predIndex) {
		case 4:
			return precpred(_ctx, 12);
		case 5:
			return precpred(_ctx, 11);
		case 6:
			return precpred(_ctx, 8);
		case 7:
			return precpred(_ctx, 7);
		case 8:
			return precpred(_ctx, 6);
		}
		return true;
	}
	private boolean argumentlist_sempred(ArgumentlistContext _localctx, int predIndex) {
		switch (predIndex) {
		case 9:
			return precpred(_ctx, 1);
		}
		return true;
	}

	public static final String _serializedATN =
		"\u0004\u0001\'\u0116\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b\u0002"+
		"\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007\u000f"+
		"\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007\u0012"+
		"\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007\u0015"+
		"\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0003\u00013\b\u0001\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0003\u0002E\b\u0002\u0001\u0002\u0001\u0002\u0005\u0002I\b\u0002\n\u0002"+
		"\f\u0002L\t\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0005\u0003U\b\u0003\n\u0003\f\u0003X\t"+
		"\u0003\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0003"+
		"\u0004e\b\u0004\u0001\u0005\u0004\u0005h\b\u0005\u000b\u0005\f\u0005i"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0003\u0006\u0094\b\u0006"+
		"\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0003\u0007\u009d\b\u0007\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0005\u0007\u00a2\b\u0007\n\u0007\f\u0007\u00a5\t\u0007\u0001\b\u0001"+
		"\b\u0001\t\u0001\t\u0001\n\u0001\n\u0003\n\u00ad\b\n\u0001\u000b\u0003"+
		"\u000b\u00b0\b\u000b\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001"+
		"\f\u0005\f\u00b9\b\f\n\f\f\f\u00bc\t\f\u0001\r\u0001\r\u0001\u000e\u0003"+
		"\u000e\u00c1\b\u000e\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0003\u000f\u00db\b\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0003\u000f\u00ec\b\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0005\u000f\u00f5\b\u000f\n"+
		"\u000f\f\u000f\u00f8\t\u000f\u0001\u0010\u0001\u0010\u0001\u0010\u0001"+
		"\u0010\u0001\u0010\u0001\u0010\u0003\u0010\u0100\b\u0010\u0001\u0011\u0003"+
		"\u0011\u0103\b\u0011\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0012\u0001\u0012\u0005\u0012\u010b\b\u0012\n\u0012\f\u0012\u010e\t\u0012"+
		"\u0001\u0013\u0001\u0013\u0001\u0014\u0001\u0014\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0000\u0006\u0004\u0006\u000e\u0018\u001e$\u0016\u0000\u0002"+
		"\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e"+
		" \"$&(*\u0000\u0003\u0001\u0000\u0011\u0013\u0001\u0000\u0017\u0018\u0002"+
		"\u0000\u0015\u0015\u0019\u0019\u012c\u0000,\u0001\u0000\u0000\u0000\u0002"+
		"2\u0001\u0000\u0000\u0000\u0004D\u0001\u0000\u0000\u0000\u0006M\u0001"+
		"\u0000\u0000\u0000\bd\u0001\u0000\u0000\u0000\ng\u0001\u0000\u0000\u0000"+
		"\f\u0093\u0001\u0000\u0000\u0000\u000e\u009c\u0001\u0000\u0000\u0000\u0010"+
		"\u00a6\u0001\u0000\u0000\u0000\u0012\u00a8\u0001\u0000\u0000\u0000\u0014"+
		"\u00ac\u0001\u0000\u0000\u0000\u0016\u00af\u0001\u0000\u0000\u0000\u0018"+
		"\u00b1\u0001\u0000\u0000\u0000\u001a\u00bd\u0001\u0000\u0000\u0000\u001c"+
		"\u00c0\u0001\u0000\u0000\u0000\u001e\u00da\u0001\u0000\u0000\u0000 \u00ff"+
		"\u0001\u0000\u0000\u0000\"\u0102\u0001\u0000\u0000\u0000$\u0104\u0001"+
		"\u0000\u0000\u0000&\u010f\u0001\u0000\u0000\u0000(\u0111\u0001\u0000\u0000"+
		"\u0000*\u0113\u0001\u0000\u0000\u0000,-\u0003\u0002\u0001\u0000-.\u0003"+
		"\u0004\u0002\u0000.\u0001\u0001\u0000\u0000\u0000/0\u0005\u0001\u0000"+
		"\u000003\u0005\u0002\u0000\u000013\u0001\u0000\u0000\u00002/\u0001\u0000"+
		"\u0000\u000021\u0001\u0000\u0000\u00003\u0003\u0001\u0000\u0000\u0000"+
		"45\u0006\u0002\uffff\uffff\u00005E\u0003\u0006\u0003\u000067\u0003\u0014"+
		"\n\u000078\u0003(\u0014\u000089\u0005\u0003\u0000\u00009:\u0003\u0016"+
		"\u000b\u0000:;\u0005\u0004\u0000\u0000;<\u0003\b\u0004\u0000<E\u0001\u0000"+
		"\u0000\u0000=>\u0003\u0014\n\u0000>?\u0003(\u0014\u0000?@\u0005\u0003"+
		"\u0000\u0000@A\u0003\u0016\u000b\u0000AB\u0005\u0004\u0000\u0000BC\u0005"+
		"\u0005\u0000\u0000CE\u0001\u0000\u0000\u0000D4\u0001\u0000\u0000\u0000"+
		"D6\u0001\u0000\u0000\u0000D=\u0001\u0000\u0000\u0000EJ\u0001\u0000\u0000"+
		"\u0000FG\n\u0001\u0000\u0000GI\u0003\u0004\u0002\u0002HF\u0001\u0000\u0000"+
		"\u0000IL\u0001\u0000\u0000\u0000JH\u0001\u0000\u0000\u0000JK\u0001\u0000"+
		"\u0000\u0000K\u0005\u0001\u0000\u0000\u0000LJ\u0001\u0000\u0000\u0000"+
		"MN\u0006\u0003\uffff\uffff\u0000NO\u0003\u0012\t\u0000OP\u0003\u000e\u0007"+
		"\u0000PQ\u0005\u0005\u0000\u0000QV\u0001\u0000\u0000\u0000RS\n\u0001\u0000"+
		"\u0000SU\u0003\u0006\u0003\u0002TR\u0001\u0000\u0000\u0000UX\u0001\u0000"+
		"\u0000\u0000VT\u0001\u0000\u0000\u0000VW\u0001\u0000\u0000\u0000W\u0007"+
		"\u0001\u0000\u0000\u0000XV\u0001\u0000\u0000\u0000YZ\u0005\u0006\u0000"+
		"\u0000Z[\u0003\u0006\u0003\u0000[\\\u0003\n\u0005\u0000\\]\u0005\u0007"+
		"\u0000\u0000]e\u0001\u0000\u0000\u0000^_\u0005\u0006\u0000\u0000_`\u0003"+
		"\n\u0005\u0000`a\u0005\u0007\u0000\u0000ae\u0001\u0000\u0000\u0000bc\u0005"+
		"\u0006\u0000\u0000ce\u0005\u0007\u0000\u0000dY\u0001\u0000\u0000\u0000"+
		"d^\u0001\u0000\u0000\u0000db\u0001\u0000\u0000\u0000e\t\u0001\u0000\u0000"+
		"\u0000fh\u0003\f\u0006\u0000gf\u0001\u0000\u0000\u0000hi\u0001\u0000\u0000"+
		"\u0000ig\u0001\u0000\u0000\u0000ij\u0001\u0000\u0000\u0000j\u000b\u0001"+
		"\u0000\u0000\u0000kl\u0003\u001e\u000f\u0000lm\u0005\u0005\u0000\u0000"+
		"m\u0094\u0001\u0000\u0000\u0000no\u0005\b\u0000\u0000op\u0005\u0003\u0000"+
		"\u0000pq\u0003\u001e\u000f\u0000qr\u0005\u0004\u0000\u0000r\u0094\u0001"+
		"\u0000\u0000\u0000st\u0005\b\u0000\u0000tu\u0005\u0003\u0000\u0000uv\u0003"+
		"\u001e\u000f\u0000vw\u0005\u0004\u0000\u0000wx\u0005\t\u0000\u0000xy\u0003"+
		"\n\u0005\u0000y\u0094\u0001\u0000\u0000\u0000z{\u0005\n\u0000\u0000{|"+
		"\u0005\u0003\u0000\u0000|}\u0003\u001c\u000e\u0000}~\u0005\u0005\u0000"+
		"\u0000~\u007f\u0003\u001c\u000e\u0000\u007f\u0080\u0005\u0005\u0000\u0000"+
		"\u0080\u0081\u0003\u001c\u000e\u0000\u0081\u0082\u0005\u0004\u0000\u0000"+
		"\u0082\u0083\u0003\n\u0005\u0000\u0083\u0094\u0001\u0000\u0000\u0000\u0084"+
		"\u0085\u0005\u000b\u0000\u0000\u0085\u0086\u0005\u0003\u0000\u0000\u0086"+
		"\u0087\u0003\u001e\u000f\u0000\u0087\u0088\u0005\u0004\u0000\u0000\u0088"+
		"\u0089\u0003\n\u0005\u0000\u0089\u0094\u0001\u0000\u0000\u0000\u008a\u008b"+
		"\u0005\f\u0000\u0000\u008b\u0094\u0005\u0005\u0000\u0000\u008c\u008d\u0005"+
		"\r\u0000\u0000\u008d\u0094\u0005\u0005\u0000\u0000\u008e\u008f\u0005\r"+
		"\u0000\u0000\u008f\u0090\u0003\u001e\u000f\u0000\u0090\u0091\u0005\u0005"+
		"\u0000\u0000\u0091\u0094\u0001\u0000\u0000\u0000\u0092\u0094\u0003\b\u0004"+
		"\u0000\u0093k\u0001\u0000\u0000\u0000\u0093n\u0001\u0000\u0000\u0000\u0093"+
		"s\u0001\u0000\u0000\u0000\u0093z\u0001\u0000\u0000\u0000\u0093\u0084\u0001"+
		"\u0000\u0000\u0000\u0093\u008a\u0001\u0000\u0000\u0000\u0093\u008c\u0001"+
		"\u0000\u0000\u0000\u0093\u008e\u0001\u0000\u0000\u0000\u0093\u0092\u0001"+
		"\u0000\u0000\u0000\u0094\r\u0001\u0000\u0000\u0000\u0095\u0096\u0006\u0007"+
		"\uffff\uffff\u0000\u0096\u009d\u0003&\u0013\u0000\u0097\u0098\u0003&\u0013"+
		"\u0000\u0098\u0099\u0005\u000e\u0000\u0000\u0099\u009a\u0005$\u0000\u0000"+
		"\u009a\u009b\u0005\u000f\u0000\u0000\u009b\u009d\u0001\u0000\u0000\u0000"+
		"\u009c\u0095\u0001\u0000\u0000\u0000\u009c\u0097\u0001\u0000\u0000\u0000"+
		"\u009d\u00a3\u0001\u0000\u0000\u0000\u009e\u009f\n\u0001\u0000\u0000\u009f"+
		"\u00a0\u0005\u0010\u0000\u0000\u00a0\u00a2\u0003\u000e\u0007\u0002\u00a1"+
		"\u009e\u0001\u0000\u0000\u0000\u00a2\u00a5\u0001\u0000\u0000\u0000\u00a3"+
		"\u00a1\u0001\u0000\u0000\u0000\u00a3\u00a4\u0001\u0000\u0000\u0000\u00a4"+
		"\u000f\u0001\u0000\u0000\u0000\u00a5\u00a3\u0001\u0000\u0000\u0000\u00a6"+
		"\u00a7\u0001\u0000\u0000\u0000\u00a7\u0011\u0001\u0000\u0000\u0000\u00a8"+
		"\u00a9\u0007\u0000\u0000\u0000\u00a9\u0013\u0001\u0000\u0000\u0000\u00aa"+
		"\u00ad\u0005\u0014\u0000\u0000\u00ab\u00ad\u0003\u0012\t\u0000\u00ac\u00aa"+
		"\u0001\u0000\u0000\u0000\u00ac\u00ab\u0001\u0000\u0000\u0000\u00ad\u0015"+
		"\u0001\u0000\u0000\u0000\u00ae\u00b0\u0003\u0018\f\u0000\u00af\u00ae\u0001"+
		"\u0000\u0000\u0000\u00af\u00b0\u0001\u0000\u0000\u0000\u00b0\u0017\u0001"+
		"\u0000\u0000\u0000\u00b1\u00b2\u0006\f\uffff\uffff\u0000\u00b2\u00b3\u0003"+
		"\u0012\t\u0000\u00b3\u00b4\u0003*\u0015\u0000\u00b4\u00ba\u0001\u0000"+
		"\u0000\u0000\u00b5\u00b6\n\u0001\u0000\u0000\u00b6\u00b7\u0005\u0010\u0000"+
		"\u0000\u00b7\u00b9\u0003\u0018\f\u0002\u00b8\u00b5\u0001\u0000\u0000\u0000"+
		"\u00b9\u00bc\u0001\u0000\u0000\u0000\u00ba\u00b8\u0001\u0000\u0000\u0000"+
		"\u00ba\u00bb\u0001\u0000\u0000\u0000\u00bb\u0019\u0001\u0000\u0000\u0000"+
		"\u00bc\u00ba\u0001\u0000\u0000\u0000\u00bd\u00be\u0001\u0000\u0000\u0000"+
		"\u00be\u001b\u0001\u0000\u0000\u0000\u00bf\u00c1\u0003\u001e\u000f\u0000"+
		"\u00c0\u00bf\u0001\u0000\u0000\u0000\u00c0\u00c1\u0001\u0000\u0000\u0000"+
		"\u00c1\u001d\u0001\u0000\u0000\u0000\u00c2\u00c3\u0006\u000f\uffff\uffff"+
		"\u0000\u00c3\u00db\u0005$\u0000\u0000\u00c4\u00db\u0005%\u0000\u0000\u00c5"+
		"\u00c6\u0005\u0015\u0000\u0000\u00c6\u00db\u0003\u001e\u000f\u000e\u00c7"+
		"\u00c8\u0005\u0016\u0000\u0000\u00c8\u00db\u0003\u001e\u000f\r\u00c9\u00db"+
		"\u0005\u001a\u0000\u0000\u00ca\u00db\u0005\u001b\u0000\u0000\u00cb\u00cc"+
		"\u0005\u0003\u0000\u0000\u00cc\u00cd\u0003\u001e\u000f\u0000\u00cd\u00ce"+
		"\u0005\u0004\u0000\u0000\u00ce\u00db\u0001\u0000\u0000\u0000\u00cf\u00db"+
		"\u0003 \u0010\u0000\u00d0\u00d1\u0003(\u0014\u0000\u00d1\u00d2\u0005\u0003"+
		"\u0000\u0000\u00d2\u00d3\u0003\"\u0011\u0000\u00d3\u00d4\u0005\u0004\u0000"+
		"\u0000\u00d4\u00db\u0001\u0000\u0000\u0000\u00d5\u00d6\u0003 \u0010\u0000"+
		"\u00d6\u00d7\u0005\u001f\u0000\u0000\u00d7\u00d8\u0003\u001e\u000f\u0002"+
		"\u00d8\u00db\u0001\u0000\u0000\u0000\u00d9\u00db\u0003*\u0015\u0000\u00da"+
		"\u00c2\u0001\u0000\u0000\u0000\u00da\u00c4\u0001\u0000\u0000\u0000\u00da"+
		"\u00c5\u0001\u0000\u0000\u0000\u00da\u00c7\u0001\u0000\u0000\u0000\u00da"+
		"\u00c9\u0001\u0000\u0000\u0000\u00da\u00ca\u0001\u0000\u0000\u0000\u00da"+
		"\u00cb\u0001\u0000\u0000\u0000\u00da\u00cf\u0001\u0000\u0000\u0000\u00da"+
		"\u00d0\u0001\u0000\u0000\u0000\u00da\u00d5\u0001\u0000\u0000\u0000\u00da"+
		"\u00d9\u0001\u0000\u0000\u0000\u00db\u00f6\u0001\u0000\u0000\u0000\u00dc"+
		"\u00dd\n\f\u0000\u0000\u00dd\u00de\u0007\u0001\u0000\u0000\u00de\u00f5"+
		"\u0003\u001e\u000f\r\u00df\u00e0\n\u000b\u0000\u0000\u00e0\u00e1\u0007"+
		"\u0002\u0000\u0000\u00e1\u00f5\u0003\u001e\u000f\f\u00e2\u00eb\n\b\u0000"+
		"\u0000\u00e3\u00ec\u0005\u001c\u0000\u0000\u00e4\u00ec\u0005\u001d\u0000"+
		"\u0000\u00e5\u00ec\u0005\u001e\u0000\u0000\u00e6\u00e7\u0005\u001e\u0000"+
		"\u0000\u00e7\u00ec\u0005\u001f\u0000\u0000\u00e8\u00ec\u0005 \u0000\u0000"+
		"\u00e9\u00ea\u0005 \u0000\u0000\u00ea\u00ec\u0005\u001f\u0000\u0000\u00eb"+
		"\u00e3\u0001\u0000\u0000\u0000\u00eb\u00e4\u0001\u0000\u0000\u0000\u00eb"+
		"\u00e5\u0001\u0000\u0000\u0000\u00eb\u00e6\u0001\u0000\u0000\u0000\u00eb"+
		"\u00e8\u0001\u0000\u0000\u0000\u00eb\u00e9\u0001\u0000\u0000\u0000\u00ec"+
		"\u00ed\u0001\u0000\u0000\u0000\u00ed\u00f5\u0003\u001e\u000f\t\u00ee\u00ef"+
		"\n\u0007\u0000\u0000\u00ef\u00f0\u0005!\u0000\u0000\u00f0\u00f5\u0003"+
		"\u001e\u000f\b\u00f1\u00f2\n\u0006\u0000\u0000\u00f2\u00f3\u0005\"\u0000"+
		"\u0000\u00f3\u00f5\u0003\u001e\u000f\u0007\u00f4\u00dc\u0001\u0000\u0000"+
		"\u0000\u00f4\u00df\u0001\u0000\u0000\u0000\u00f4\u00e2\u0001\u0000\u0000"+
		"\u0000\u00f4\u00ee\u0001\u0000\u0000\u0000\u00f4\u00f1\u0001\u0000\u0000"+
		"\u0000\u00f5\u00f8\u0001\u0000\u0000\u0000\u00f6\u00f4\u0001\u0000\u0000"+
		"\u0000\u00f6\u00f7\u0001\u0000\u0000\u0000\u00f7\u001f\u0001\u0000\u0000"+
		"\u0000\u00f8\u00f6\u0001\u0000\u0000\u0000\u00f9\u0100\u0003&\u0013\u0000"+
		"\u00fa\u00fb\u0003&\u0013\u0000\u00fb\u00fc\u0005\u000e\u0000\u0000\u00fc"+
		"\u00fd\u0003\u001e\u000f\u0000\u00fd\u00fe\u0005\u000f\u0000\u0000\u00fe"+
		"\u0100\u0001\u0000\u0000\u0000\u00ff\u00f9\u0001\u0000\u0000\u0000\u00ff"+
		"\u00fa\u0001\u0000\u0000\u0000\u0100!\u0001\u0000\u0000\u0000\u0101\u0103"+
		"\u0003$\u0012\u0000\u0102\u0101\u0001\u0000\u0000\u0000\u0102\u0103\u0001"+
		"\u0000\u0000\u0000\u0103#\u0001\u0000\u0000\u0000\u0104\u0105\u0006\u0012"+
		"\uffff\uffff\u0000\u0105\u0106\u0003\u001e\u000f\u0000\u0106\u010c\u0001"+
		"\u0000\u0000\u0000\u0107\u0108\n\u0001\u0000\u0000\u0108\u0109\u0005\u0010"+
		"\u0000\u0000\u0109\u010b\u0003$\u0012\u0002\u010a\u0107\u0001\u0000\u0000"+
		"\u0000\u010b\u010e\u0001\u0000\u0000\u0000\u010c\u010a\u0001\u0000\u0000"+
		"\u0000\u010c\u010d\u0001\u0000\u0000\u0000\u010d%\u0001\u0000\u0000\u0000"+
		"\u010e\u010c\u0001\u0000\u0000\u0000\u010f\u0110\u0005#\u0000\u0000\u0110"+
		"\'\u0001\u0000\u0000\u0000\u0111\u0112\u0005#\u0000\u0000\u0112)\u0001"+
		"\u0000\u0000\u0000\u0113\u0114\u0005#\u0000\u0000\u0114+\u0001\u0000\u0000"+
		"\u0000\u00142DJVdi\u0093\u009c\u00a3\u00ac\u00af\u00ba\u00c0\u00da\u00eb"+
		"\u00f4\u00f6\u00ff\u0102\u010c";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}