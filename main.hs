---- Lexical Analysis
----
{-# LANGUAGE OverloadedStrings #-}
module LexingFSM where

import Control.Monad.IO.Class
import qualified Data.Text.IO as T
import System.Environment (getArgs)
import System.IO (openFile, IOMode(ReadMode), hGetContents)
import Data.Typeable (typeOf)
import Control.Monad      (foldM)
import Data.List.NonEmpty
import Data.Text          (Text)
import Text.Printf        (printf)


---- Tokens : Keywords, Operators, Strings, Constants, Special Characters, Identifiers.
data Token = Key | Ops | Str | Const | SChar | Ident

instance Show Token where 
    show Key = "Keyword"
    show Ops = "Operator"
    show Str = "String"
    show Const = "Constant"
    show SChar = "Special Character"
    show Ident = "Identifier"

-- -- Lexer
-- lexer :: Char -> Token
-- lexer c 
--     | (c == ','|| c == '.'|| c == ';'|| c ==':'|| c =='?'|| c =='*') = SChar
--     | otherwise = Str 

-- -- Iterate over every single character.
-- l c = [ lexer x | x <- c ]

-- main = do
--     args <- getArgs
--     file <- openFile (head args) ReadMode
--     prog <- hGetContents file
--     print prog
--     print (typeOf prog)
--     print (l prog)
--     print (length (l prog))


data LexingState 
    = Init 
    | ReturnToken 
    deriving (Show, Eq)

data LexingEvent 
    = GetToken
    deriving (Show, Eq)

type FSM s e = s -> e -> IO s
lexer :: FSM LexingState LexingEvent 

-- lexer Init (GetToken token) =
--   return (ReturnToken (token :| []))
--

lexer state _ = return state

runFsm :: Foldable f => FSM s e -> s -> f e -> IO s
runFsm = foldM

withLogging
  :: (Show s, Show e)
  => FSM s e
  -> FSM s e
withLogging fsm s e = do
  s' <- fsm s e
  printf "- %s × %s → %s\n" (show s) (show e) (show s')
  return s'
