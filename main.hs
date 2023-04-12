{-# LANGUAGE OverloadedStrings          #-}
{-# LANGUAGE GADTs                      #-}
{-# LANGUAGE GeneralizedNewtypeDeriving #-}
{-# LANGUAGE TypeFamilies               #-}
module Main where

import           Control.Monad.IO.Class
import           Data.Typeable          (typeOf)
import           Data.Text hiding (head)
import qualified Data.Text (pack)
import           Data.List.NonEmpty hiding (head)
import           Data.Semigroup
import           System.Environment     (getArgs)
import           System.IO              (openFile, IOMode(ReadMode), hGetContents)
-- import qualified Data.Text.IO           as T

---- Tokens : Keywords, Operators, Strings, Constants, Special Characters, Identifiers.
data Token = Key | Ops | Str | Const | SChar | Ident

instance Show Token where 
    show Key = "Keyword"
    show Ops = "Operator"
    show Str = "String"
    show Const = "Constant"
    show SChar = "Special Character"
    show Ident = "Identifier"

-- States (phantom types)
data Init
data ReadProg
data Lexing

-- Protocol
-- Events are specified as type class methods, 
-- where method type signatures describe state transitions.
class Lexer m where 
    type State m :: * -> *
    initial      :: m (State m Init)
    readIn       :: State m Init -> m (State m ReadProg)
    -- lexP         :: State m ReadProg -> m (State m Lexing)
    end          :: State m ReadProg -> m Text

-- Driving the state machine
readProg :: (Lexer m, MonadIO m) => State m Init -> m (State m ReadProg)
readProg = readIn
    -- args <- getArgs
    -- file <- openFile (head args) ReadMode
    -- prog <- hGetContents file 
    -- >>= readIn (prog)

run :: (Lexer m, MonadIO m) => m Text
run =
    initial >>= readProg >>= end

newtype LexerT m a = LexerT 
    { runLexerT :: m a } deriving ( Functor, Monad, Applicative, MonadIO)

data LexerState s where
    Init :: LexerState Init
    ReadProg :: Text -> LexerState ReadProg 

instance (MonadIO m) => Lexer (LexerT m) where
    type State (LexerT m) = LexerState
    initial = return Init

    readIn = do
        -- args <- getArgs
        -- file <- openFile (head args) ReadMode
        -- prog <- hGetContents file 
        prog <- pack "\55555"
        return (ReadProg prog)

    end (ReadProg prog) = return prog

main :: IO ()
main = do 
    -- args <- getArgs
    -- file <- openFile (head args) ReadMode
    -- prog <- hGetContents file 
    prog <- runLexerT run 
    putStrLn("\n" ++ prog)
